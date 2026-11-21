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
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicNonlinear.hpp"

//Test
#include "FEA/Test/inc/Rigid2D.hpp"

//data
static const double T = 5.00e+00;
static const double g = 9.81e+00;
static const double L1 = 1.00e+00;
static const double L2 = 1.00e+00;
static const double M1 = 1.00e+00;
static const double M2 = 1.00e+00;
static const double q1 = +M_PI / 4;
static const double q2 = -M_PI / 4;

void test::rigid2D::double_pendulum(void)
{
	//data
	fea::Model model;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::mesh::joints::Type joint;
	typedef fea::mesh::elements::Type element;
	//nodes
	const double x1 = L1 * sin(q1), y1 = -L1 * cos(q1);
	const double x2 = x1 + L2 * sin(q2), y2 = y1 - L2 * cos(q2);
	model.mesh()->create_node(0, 0, 0);
	model.mesh()->create_node(x1, y1, 0);
	model.mesh()->create_node(x1, y1, 0);
	model.mesh()->create_node(x2, y2, 0);
	//joints
	model.mesh()->create_joint(joint::Rigid2D, {0, 1});
	model.mesh()->create_joint(joint::Revolute2D, {1, 2});
	model.mesh()->create_joint(joint::Rigid2D, {2, 3});
	//elements
	model.mesh()->create_element(element::Nodal, { 1 });
	model.mesh()->create_element(element::Nodal, { 1 });
	model.mesh()->create_element(element::Nodal, { 3 });
	model.mesh()->create_element(element::Nodal, { 3 });
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->inertia(M1);
	((fea::mesh::elements::Nodal*) model.mesh()->element(1))->inertia(M1);
	((fea::mesh::elements::Nodal*) model.mesh()->element(2))->inertia(M2);
	((fea::mesh::elements::Nodal*) model.mesh()->element(3))->inertia(M2);
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->dof(dof::Translation_1);
	((fea::mesh::elements::Nodal*) model.mesh()->element(1))->dof(dof::Translation_2);
	((fea::mesh::elements::Nodal*) model.mesh()->element(2))->dof(dof::Translation_1);
	((fea::mesh::elements::Nodal*) model.mesh()->element(3))->dof(dof::Translation_2);
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//loads
	model.boundary()->create_load_case();
	model.boundary()->load_case(0)->create_load_node(1, dof::Translation_2, -M1 * g);
	model.boundary()->load_case(0)->create_load_node(3, dof::Translation_2, -M2 * g);
	model.boundary()->create_load_combination(0, false, 1);
	//setup
	model.analysis()->solver_dynamic_nonlinear()->enable();
	model.analysis()->solver_dynamic_nonlinear()->silent(true);
	model.analysis()->solver_dynamic_nonlinear()->time_max(T);
	model.analysis()->solver_dynamic_nonlinear()->step_max(2000);
	model.analysis()->solver_dynamic_nonlinear()->load_combination(0);
	model.analysis()->solver_dynamic_nonlinear()->watch_dof().node(3);
	model.analysis()->solver_dynamic_nonlinear()->watch_dof().dof(dof::Translation_2);
	//solve
	model.solve();
	//save
	model.analysis()->solver_dynamic_nonlinear()->save("Test/data/Rigid 2D/Double Pendulum/data.txt", {
		{1, dof::Translation_1}, {1, dof::Translation_2}, {3, dof::Translation_1}, {3, dof::Translation_2}
	});
	//validator
	for(uint32_t i = 0; i < 4; i++)
	{
		validator.create_item();
		validator.item(i)->load_numeric("Test/data/Rigid 2D/Double Pendulum/data.txt", 12, 3 * i);
		validator.item(i)->load_reference("Test/data/Rigid 2D/Double Pendulum/reference.dat", 0, i + 1);
	}
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Rigid 2D/Double Pendulum/plot.gp");
}