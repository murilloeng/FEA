//std
#include <cmath>

//Math
#include "Math/inc/Validation/Validator.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Joints/Type.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Nodal/Nodal.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicNonlinear.hpp"

//Test
#include "FEA/Test/inc/Rigid2D.hpp"

//data
static const double np = 2;
static const double L = 1.00e+00;
static const double M = 1.00e+00;
static const double g = 9.81e+00;

void test::rigid2D::pendulum(void)
{
	//data
	fea::Model model;
	math::validation::Validator validator;
	const double T = 4 * std::comp_ellint_1(sin(M_PI / 4)) / sqrt(g / L);
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::mesh::joints::Type joint;
	typedef fea::mesh::elements::Type element;
	//nodes
	model.mesh()->create_node(0, 0, 0);
	model.mesh()->create_node(L, 0, 0);
	//joints
	model.mesh()->create_joint(joint::Rigid2D, {0, 1});
	//elements
	model.mesh()->create_element(element::Nodal, { 1 });
	model.mesh()->create_element(element::Nodal, { 1 });
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->inertia(M);
	((fea::mesh::elements::Nodal*) model.mesh()->element(1))->inertia(M);
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->dof(dof::Translation_1);
	((fea::mesh::elements::Nodal*) model.mesh()->element(1))->dof(dof::Translation_2);
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Translation_2, -M * g);
	//setup
	model.analysis()->solver_dynamic_nonlinear()->enable();
	model.analysis()->solver_dynamic_nonlinear()->silent(true);
	model.analysis()->solver_dynamic_nonlinear()->step_max(2000);
	model.analysis()->solver_dynamic_nonlinear()->time_max(np * T);
	model.analysis()->solver_dynamic_nonlinear()->load_combination(0);
	model.analysis()->solver_dynamic_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_dynamic_nonlinear()->watch_dof().dof(dof::Translation_2);
	//solve
	model.solve();
	//save
	model.analysis()->solver_dynamic_nonlinear()->save("Test/data/Rigid 2D/Pendulum/data.txt", {
		{1, dof::Translation_1}, {1, dof::Translation_2}
	});
	//validator
	validator.create_item();
	validator.create_item();
	validator.item(0)->load_numeric("Test/data/Rigid 2D/Pendulum/data.txt", 6, 0);
	validator.item(1)->load_numeric("Test/data/Rigid 2D/Pendulum/data.txt", 6, 3);
	validator.item(0)->load_reference("Test/data/Rigid 2D/Pendulum/reference.dat", 0, 1);
	validator.item(1)->load_reference("Test/data/Rigid 2D/Pendulum/reference.dat", 0, 2);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
}