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

#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t ne = 5;
static const double L = 1.294e+01;
static const double v = 3.000e-01;
static const double E = 1.000e+00;
static const double A = 1.885e+06;
static const double I = 9.270e+03;
static const double b = 2.470e-02;

static double function(double v)
{
	v *= -1;
	const double P = E * A / L * (v * sin(b) - 0.6 * v * v / L);
	const double p = fmax(P, 0) / (M_PI * M_PI * E * I / L / L);
	const double w = p != 0 ? M_PI_2 * sqrt(p) / tan(M_PI_2 * sqrt(p)) : 1;
	const double d1 = p != 0 ? (M_PI * M_PI * p / 4 / (1 - w) + w) / 2 : 2;
	const double d2 = d1 - w;
	const double d3 = d1 + d2;
	const double d4 = d3 / 3;
	const double d5 = 2 * d4 * w;
	const double F = 6 * E * I / L / L * d5 * v / L;
	return 2 * (F + P * sin(b));
}

//reference: https://doi.org/10.1093/qjmam/17.4.451

void test::beam2D::elastic::williams_toggle(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	std::function<void(int32_t, size_t, const size_t*)> generate = [&model](int32_t type, size_t ne, const size_t* nodes)
	{
		if(type != 1) return;
		for(uint32_t i = 0; i < ne; i++)
		{
			const uint32_t n1 = nodes[2 * i + 0] - 1;
			const uint32_t n2 = nodes[2 * i + 1] - 1;
			model.mesh()->create_element(fea::mesh::elements::Type::Beam2D, {n1, n2});
		}
	};
	//points
	model.geometry()->create_point(0, 0, 0);
	model.geometry()->create_point(-L * cos(b), -L * sin(b), 0);
	model.geometry()->create_point(+L * cos(b), -L * sin(b), 0);
	//curves
	model.geometry()->create_line(0, 1);
	model.geometry()->create_line(0, 2);
	model.geometry()->curve(0)->structured(ne);
	model.geometry()->curve(1)->structured(ne);
	model.geometry()->curve(0)->generate_elements(generate);
	model.geometry()->curve(1)->generate_elements(generate);
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
	model.boundary()->create_support(1, dof::Rotation_3);
	model.boundary()->create_support(2, dof::Rotation_3);
	model.boundary()->create_support(1, dof::Translation_1);
	model.boundary()->create_support(2, dof::Translation_1);
	model.boundary()->create_support(1, dof::Translation_2);
	model.boundary()->create_support(2, dof::Translation_2);
	//loads
	section.area(A);
	section.inertia(1, I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(0, dof::Translation_2, -1);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->step_size(5.00e-01);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->stop_criteria().state_min(-L * sin(b) / 0.6);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::StateLimitMinimum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Williams Toggle");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Williams Toggle/data.txt", {{0, dof::Translation_2}});
	//validator
	validator.create_item();
	validator.item(0)->function(function);
	validator.item(0)->tolerance(1.15e-02);
	validator.item(0)->load_numeric("Test/data/Beam 2D/Williams Toggle/data.txt", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
}