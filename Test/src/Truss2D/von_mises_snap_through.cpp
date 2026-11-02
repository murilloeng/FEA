//std
#include <cmath>

//Test
#include "FEA/Test/inc/Truss2D.hpp"

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
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

using dof = fea::mesh::nodes::DOF;
using strain = fea::mesh::elements::StrainMeasure;
using continuation = math::solvers::Continuation::Type;

/*
	von Mises truss with a spring at the apex (Crisfield, Vol. 1, Chapter 1), the spring being a vertical third bar
	from the apex down to a support. With y = H + u and the Quadratic measure:
		P_bars = -EA * y * (y * y - H * H) / L^3
		P_spring = -N_s, N_s = Ks * Ls * s * (s * s - 1) / 2, s = 1 + u / Ls
	The stiffness Ks is half of EA * H^2 / L^3, the softening slope at y = 0, so limit points remain: snap-through.
*/

//data
static const double H = 1.00e-01;
static const double R = 1.00e+00;
static const double E = 2.10e+11;
static const double A = 1.00e-03;
static const double Ls = 1.00e+00;
static const double L = sqrt(H * H + R * R);
static const double P = E * A * pow(H / L, 3);
static const double Ks = 5.00e-01 * E * A * H * H / pow(L, 3);

static double function(double u)
{
	const double s = 1 + u / Ls;
	const double Pb = -(1 + u / H) * (2 + u / H) * u / H * P;
	const double Ps = -Ks * Ls * s * (s * s - 1) / 2;
	return (Pb + Ps) / P;
}

void test::truss2D::von_mises_snap_through(void)
{
	//data
	fea::Model model;
	sections::Generic bars, spring;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//nodes
	model.mesh()->create_node(-R, 0, 0);
	model.mesh()->create_node(+0, H, 0);
	model.mesh()->create_node(+R, 0, 0);
	model.mesh()->create_node(+0, H - Ls, 0);
	//elements
	bars.area(A);
	spring.area(Ks * Ls / E);
	material.elastic_modulus(E);
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {0, 1});
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {1, 2});
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {3, 1});
	((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->section(&bars);
	((fea::mesh::elements::Truss2D*) model.mesh()->element(1))->section(&bars);
	((fea::mesh::elements::Truss2D*) model.mesh()->element(2))->section(&spring);
	for(uint32_t i = 0; i < 3; i++)
	{
		((fea::mesh::elements::Truss2D*) model.mesh()->element(i))->material(&material);
	}
	fea::mesh::elements::Truss::strain_measure(strain::Quadratic);
	//supports
	model.boundary()->create_support(1, dof::Translation_1);
	for(uint32_t i : {0, 2, 3})
	{
		model.boundary()->create_support(i, dof::Translation_1);
		model.boundary()->create_support(i, dof::Translation_2);
	}
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Translation_2, -P);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(800);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->continuation().type(continuation::ArcLengthCylindrical);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Truss 2D/von Mises Snap Through");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Truss 2D/von Mises Snap Through/data.txt", {{1, dof::Translation_2}});
	//validator
	validator.create_item();
	validator.item(0)->tolerance(1.00e-4);
	validator.item(0)->function(function);
	validator.item(0)->load_numeric("Test/data/Truss 2D/von Mises Snap Through/data.txt", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Truss 2D/von Mises Snap Through/plot.gp");
}
