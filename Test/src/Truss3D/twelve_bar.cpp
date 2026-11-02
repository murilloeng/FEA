//std
#include <cmath>

//Test
#include "FEA/Test/inc/Truss3D.hpp"

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
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

using dof = fea::mesh::nodes::DOF;
using strain = fea::mesh::elements::StrainMeasure;
using continuation = math::solvers::Continuation::Type;

/*
	12 bar space truss, S. Krenk (2009), Non-linear Modeling and Analysis of Solids and Structures, Example 8.3, pg. 281.
	Top nodes carry F at the center and 1.5 F at the sides. The path has snap-through and snap-back: v changes direction
	twice, and it ends mirrored at v = w = 2 h, after the prestressed state v = w = h, F = 0.
	u: side node outward displacement, v: side node downward displacement, w: center node downward displacement.
	reference.dat (u, v, w, F): independent symmetric arc-length solve of the (u, v, w, F) equilibrium equations, Green
	strain, since the book only gives the path as a figure. The 284 steps reach the mirror
	state, past which the bars stiffen in tension and the loads grow quickly.
*/

//data
static const double H = 1.000e+00;
static const double B = 1.000e+00;
static const double a = 1.414e+00;
static const double c = 1.697e+00;
static const double E = 1.000e+00;
static const double A = 1.000e+00;

void test::truss3D::space_truss(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//nodes
	for(double x : {-c, 0.0, +c})
	{
		model.mesh()->create_node(x, +B, 0);
		model.mesh()->create_node(x, -B, 0);
		model.mesh()->create_node(x == 0 ? 0 : x > 0 ? +a : -a, 0, H);
	}
	//elements
	section.area(A);
	material.elastic_modulus(E);
	const uint32_t bars[12][2] = {
		{0, 2}, {1, 2}, {3, 5}, {4, 5}, {6, 8}, {7, 8},
		{0, 5}, {1, 5}, {2, 5}, {6, 5}, {7, 5}, {8, 5}
	};
	for(uint32_t i = 0; i < 12; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Truss3D, {bars[i][0], bars[i][1]});
		((fea::mesh::elements::Truss3D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Truss3D*) model.mesh()->element(i))->material(&material);
	}
	fea::mesh::elements::Truss::strain_measure(strain::Quadratic);
	//supports
	for(uint32_t i : {0, 1, 3, 4, 6, 7})
	{
		model.boundary()->create_support(i, dof::Translation_1);
		model.boundary()->create_support(i, dof::Translation_2);
		model.boundary()->create_support(i, dof::Translation_3);
	}
	model.boundary()->create_support(2, dof::Translation_2);
	model.boundary()->create_support(8, dof::Translation_2);
	model.boundary()->create_support(5, dof::Translation_1);
	model.boundary()->create_support(5, dof::Translation_2);
	//loads
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->load_case(0)->create_load_node(5, dof::Translation_3, -1.0 * E * A);
	model.boundary()->load_case(0)->create_load_node(2, dof::Translation_3, -1.5 * E * A);
	model.boundary()->load_case(0)->create_load_node(8, dof::Translation_3, -1.5 * E * A);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(284);
	model.analysis()->solver_static_nonlinear()->step_size(1.00e-02);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(5);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_3);
	model.analysis()->solver_static_nonlinear()->continuation().type(continuation::ArcLengthCylindrical);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Truss 3D/Space Truss");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Truss 3D/Space Truss/data.txt", {
		{8, dof::Translation_1}, {8, dof::Translation_3}, {2, dof::Translation_3}, {5, dof::Translation_3}
	});
	//validator
	const auto adjust = [](double& x1, double&){ x1 = -x1; };
	const auto adjust_both = [](double& x1, double& x2){ x1 = -x1; x2 = -x2; };
	validator.create_item();
	validator.create_item();
	validator.create_item();
	validator.item(0)->tolerance(2.00e-4);
	validator.item(1)->tolerance(2.00e-4);
	validator.item(2)->tolerance(2.00e-4);
	validator.item(0)->adjuster_numeric(adjust);
	validator.item(1)->adjuster_numeric(adjust);
	validator.item(2)->adjuster_numeric(adjust_both);
	validator.item(0)->load_numeric("Test/data/Truss 3D/Space Truss/data.txt", 3, 4);
	validator.item(1)->load_numeric("Test/data/Truss 3D/Space Truss/data.txt", 1, 4);
	validator.item(2)->load_numeric("Test/data/Truss 3D/Space Truss/data.txt", 3, 1);
	validator.item(0)->load_reference("Test/data/Truss 3D/Space Truss/reference.dat", 2, 3);
	validator.item(1)->load_reference("Test/data/Truss 3D/Space Truss/reference.dat", 1, 3);
	validator.item(2)->load_reference("Test/data/Truss 3D/Space Truss/reference.dat", 2, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Truss 3D/Space Truss/plot.gp");
}
