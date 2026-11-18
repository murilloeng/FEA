//std
#include <cmath>

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Validation/Validator.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t nw = 10;
static const uint32_t nh = 40;
static const uint32_t ne = 10;
static const double w = 2.00e-01;
static const double h = 4.00e-01;
static const double L = 3.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double K = 0.00e+00;
static const double sy = 2.50e+08;

static const double ky = 2 * sy / E / h;
static const double I = w * h * h * h / 12;
static const double My = sy * w * h * h / 6;

//reference: Lubliner, Plasticity Theory, sec. 4.4, ISBN 978-0-486-46290-5

static double function_t3(double p)
{
	const double M = 1.5 * My * p;
	return (M <= My ? M / E / I : ky / sqrt(3 - 2 * M / My)) * L / 2;
}
static double function_u1(double p)
{
	const double t = function_t3(p);
	return t == 0 ? 0 : L * (sin(t) / t - 1);
}

void test::beam3D::inelastic::pinned_bending(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//points
	model.geometry()->create_point(0, 0, 0);
	model.geometry()->create_point(L, 0, 0);
	//curves
	model.geometry()->create_line(0, 1);
	model.geometry()->curve(0)->structured(ne);
	model.geometry()->curve(0)->element_type(fea::mesh::elements::Type::Beam3D);
	//generate
	model.geometry()->generate_mesh();
	//elements
	section.width(w);
	section.height(h);
	section.fibers_width(nw);
	section.fibers_height(nh);
	material.yield_stress(sy);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	material.plastic_modulus(K);
	materials::Mechanic::inelastic(true);
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam3D*) element)->section(&section);
		((fea::mesh::elements::Beam3D*) element)->material(&material);
	}
	//supports
	model.boundary()->create_support(0, dof::Rotation_1);
	model.boundary()->create_support(0, dof::Rotation_2);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(0, dof::Translation_3);
	model.boundary()->create_support(1, dof::Rotation_1);
	model.boundary()->create_support(1, dof::Rotation_2);
	model.boundary()->create_support(1, dof::Translation_2);
	model.boundary()->create_support(1, dof::Translation_3);
	//loads
	section.compute();
	const double W = section.plastic_modulus(1);
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->load_case(0)->create_load_node(0, dof::Rotation_3, -sy * W);
	model.boundary()->load_case(0)->create_load_node(1, dof::Rotation_3, +sy * W);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(500);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_1);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::MinimalNorm);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Inelastic/Pinned Bending");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Inelastic/Pinned Bending/data.txt", {{1, dof::Translation_1}});
	//validator
	validator.create_item();
	validator.item(0)->function(function_u1);
	validator.item(0)->load_numeric("Test/data/Beam 3D/Inelastic/Pinned Bending/data.txt", 1, 0);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Inelastic/Pinned Bending/plot.gp");
}