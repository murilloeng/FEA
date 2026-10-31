//std
#include <cmath>

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Quadrature/Rules.hpp"
#include "Math/inc/Validation/Validator.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Draw.hpp"
#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t ne = 10;
static const uint32_t nw = 1;
static const uint32_t nh = 40;
static const double w = 2.00e-01;
static const double h = 4.00e-01;
static const double L = 3.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double K = 0.00e+00;
static const double sy = 2.50e+08;

static const double I = w * h * h * h / 12;
static const double My = sy * w * h * h / 6;
static const double ky = 2 * sy / E / h;

//reference: elastic-perfectly-plastic rectangle, k = M / EI for M <= My and k = ky / sqrt(3 - 2 M / My)
//for My <= M < Mp = 1.5 My (e.g. Lubliner, Plasticity Theory, sec. 4.4), small displacements
//by symmetry each half is a cantilever of length l = L / 2 with tip force Q = P / 2, so Q l = p Mp; with r the
//distance to the support, M = Q r, ry = My / Q the elastic-plastic boundary and a = Q / My, the mid-span
//deflection v = -int k r dr is in closed form
static double function_u2(double p)
{
	const double l = L / 2;
	const double Q = 1.5 * My * p / l, a = Q / My, ry = fmin(1 / a, l);
	if(p == 0) return 0;
	const double s = fmax(3 - 2 * a * l, 0);
	const double ve = Q * ry * ry * ry / 3 / E / I;
	const double vp = ry < l ? ky / 4 / a / a * (16.0 / 3 - 6 * sqrt(s) + 2 * s * sqrt(s) / 3) : 0;
	return -(ve + vp);
}

void test::beam2D::inelastic::pinned_force(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//nodes (each half graded towards mid-span, where the curvature becomes singular at collapse: distance to
	//mid-span (L / 2) (k / ne)^3; node 0 left support, node 1 right support, node 2 mid-span)
	model.mesh()->create_node(0, 0, 0);
	model.mesh()->create_node(L, 0, 0);
	model.mesh()->create_node(L / 2, 0, 0);
	for(uint32_t k = 1; k < ne; k++)
	{
		model.mesh()->create_node(L / 2 * (1 - pow(double(k) / ne, 3)), 0, 0);
		model.mesh()->create_node(L / 2 * (1 + pow(double(k) / ne, 3)), 0, 0);
	}
	//elements
	for(uint32_t k = 0; k < ne; k++)
	{
		const uint32_t a = k == 0 ? 2 : 2 * k + 1, b = k + 1 == ne ? 0 : 2 * k + 3;
		model.mesh()->create_element(fea::mesh::elements::Type::Beam2D, {a, b});
		model.mesh()->create_element(fea::mesh::elements::Type::Beam2D, {k == 0 ? 2 : a + 1, k + 1 == ne ? 1 : b + 1});
	}
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
		((fea::mesh::elements::Beam2D*) element)->section(&section);
		((fea::mesh::elements::Beam2D*) element)->material(&material);
	}
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(1, dof::Translation_2);
	//loads
	section.compute();
	const double W = section.plastic_modulus(1);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(2, dof::Translation_2, -4 * sy * W / L);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(2);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::MinimalNorm);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(1.00e+00);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Inelastic/Pinned Force");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Inelastic/Pinned Force/data.txt", {{2, dof::Translation_2}});
	//validator
	validator.create_item();
	validator.item(0)->function(function_u2);
	validator.item(0)->load_numeric("Test/data/Beam 2D/Inelastic/Pinned Force/data.txt", 1, 0);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 2D/Inelastic/Pinned Force/plot.gp");
}