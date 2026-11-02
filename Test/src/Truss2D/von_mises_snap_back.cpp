//std
#include <cmath>
#include <vector>

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
	von Mises truss loaded through a spring in series (Crisfield, Vol. 1, Chapter 1), the spring being a vertical third
	bar from the apex up to the loaded node. The load is the same as in the bars, so with the Quadratic measure:
		P = -EA * y * (y * y - H * H) / L^3, y = H + u
		-P = Ks * Ls * s * (s * s - 1) / 2, s = 1 + (v - u) / Ls
	where u is the apex and v the loaded node displacement. A spring softer than EA * H^2 / L^3 gives snap-back in v.
	Two exact references: load vs u from a function, and load vs v from the parametric curve u -> (v, P).
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
	return -(1 + u / H) * (2 + u / H) * u / H;
}
static double spring_stretch(double N)
{
	double s = 1;
	for(uint32_t i = 0; i < 50; i++)
	{
		const double r = Ks * Ls * s * (s * s - 1) / 2 - N;
		s -= r / (Ks * Ls * (3 * s * s - 1) / 2);
	}
	return s;
}

void test::truss2D::von_mises_snap_back(void)
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
	model.mesh()->create_node(+0, H + Ls, 0);
	//elements
	bars.area(A);
	spring.area(Ks * Ls / E);
	material.elastic_modulus(E);
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {0, 1});
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {1, 2});
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {1, 3});
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
	model.boundary()->create_support(3, dof::Translation_1);
	for(uint32_t i : {0, 2})
	{
		model.boundary()->create_support(i, dof::Translation_1);
		model.boundary()->create_support(i, dof::Translation_2);
	}
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(3, dof::Translation_2, -P);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(3);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->continuation().type(continuation::ArcLengthCylindrical);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Truss 2D/von Mises Snap Back");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Truss 2D/von Mises Snap Back/data.txt", {{1, dof::Translation_2}, {3, dof::Translation_2}});
	//reference
	const uint32_t nr = 401;
	std::vector<double> vr(nr), pr(nr);
	for(uint32_t i = 0; i < nr; i++)
	{
		const double u = -2 * H * i / (nr - 1);
		pr[i] = function(u);
		vr[i] = u + Ls * (spring_stretch(-pr[i] * P) - 1);
	}
	//validator
	validator.create_item();
	validator.item(0)->tolerance(1.00e-4);
	validator.item(0)->function(function);
	validator.item(0)->load_numeric("Test/data/Truss 2D/von Mises Snap Back/data.txt", 0, 2);
	validator.create_item();
	validator.item(1)->tolerance(1.00e-4);
	validator.item(1)->load_reference(vr.data(), pr.data(), nr);
	validator.item(1)->load_numeric("Test/data/Truss 2D/von Mises Snap Back/data.txt", 1, 2);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Truss 2D/von Mises Snap Back/plot.gp");
}
