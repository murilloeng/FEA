//std
#include <cmath>

//Sections
#include "Sections/inc/Generic.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Validation/Validator.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t ne = 10;
static const double b = 1.00e-01;
static const double h = 1.00e-01;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;

static const double A = b * h;
static const double I = b * h * h * h / 12;

static double function_t3(double t)
{
	return 2 * M_PI * t;
}
static double function_u1(double t)
{
	t *= 2 * M_PI;
	return t == 0 ? 0 : L * (sin(t) / t - 1);
}
static double function_u2(double t)
{
	t *= 2 * M_PI;
	return t == 0 ? 0 : L * (1 - cos(t)) / t;
}

void test::beam2D::elastic::cantilever_bending(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	//points
	model.geometry()->create_point(0, 0, 0);
	model.geometry()->create_point(L, 0, 0);
	//curves
	model.geometry()->create_line(0, 1);
	model.geometry()->curve(0)->structured(ne);
	model.geometry()->curve(0)->element_type(fea::mesh::elements::Type::Beam2D);
	//generate
	model.geometry()->generate_mesh();
	//elements
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam2D*) element)->section(&section);
		((fea::mesh::elements::Beam2D*) element)->material(&material);
	}
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//supports
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//loads
	section.area(A);
	section.inertia(1, I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Rotation_3, 2 * M_PI * E * I /  L);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Rotation_3);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Cantilever Bending");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Cantilever Bending/data.txt", {
		{1, dof::Translation_1}, {1, dof::Translation_2}, {1, dof::Rotation_3}
	});
	//validator
	validator.create_item();
	validator.create_item();
	validator.create_item();
	validator.item(0)->function(function_u1);
	validator.item(1)->function(function_u2);
	validator.item(2)->function(function_t3);
	validator.item(0)->load_numeric("Test/data/Beam 2D/Cantilever Bending/data.txt", 3, 0);
	validator.item(1)->load_numeric("Test/data/Beam 2D/Cantilever Bending/data.txt", 3, 1);
	validator.item(2)->load_numeric("Test/data/Beam 2D/Cantilever Bending/data.txt", 3, 2);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
}