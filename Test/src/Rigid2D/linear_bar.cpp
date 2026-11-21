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
#include "FEA/inc/Analysis/Solvers/DynamicLinear.hpp"

//Test
#include "FEA/Test/inc/Rigid2D.hpp"

//data
static const double np = 5;
static const double L = 1.00e+00;
static const double M = 1.00e+00;
static const double K = 4 * M_PI * M_PI;
static const double P = 1.00e-03;

static double function_t(double t)
{
	return P * L / K * (1 - cos(sqrt(K / M) / L * t));
}
static double function_u(double t)
{
	return L * function_t(t);
}

void test::rigid2D::linear_bar(void)
{
	//data
	fea::Model model;
	math::validation::Validator validator;
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
	model.mesh()->create_element(element::Nodal, { 0 });
	model.mesh()->create_element(element::Nodal, { 1 });
	model.mesh()->create_element(element::Nodal, { 1 });
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->stiffness(K);
	((fea::mesh::elements::Nodal*) model.mesh()->element(1))->inertia(M);
	((fea::mesh::elements::Nodal*) model.mesh()->element(2))->inertia(M);
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->dof(dof::Rotation_3);
	((fea::mesh::elements::Nodal*) model.mesh()->element(1))->dof(dof::Translation_1);
	((fea::mesh::elements::Nodal*) model.mesh()->element(2))->dof(dof::Translation_2);
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Translation_2, P);
	//setup
	model.analysis()->solver_dynamic_linear()->enable();
	model.analysis()->solver_dynamic_linear()->silent(true);
	model.analysis()->solver_dynamic_linear()->step_max(1000);
	model.analysis()->solver_dynamic_linear()->load_combination(0);
	model.analysis()->solver_dynamic_linear()->watch_dof().node(1);
	model.analysis()->solver_dynamic_linear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_dynamic_linear()->time_max(np * 2 * M_PI * L * sqrt(M / K));
	//solve
	model.solve();
	//save
	model.analysis()->solver_dynamic_linear()->save("Test/data/Rigid 2D/Linear Bar/data.txt", {
		{0, dof::Rotation_3}, {1, dof::Translation_1}, {1, dof::Translation_2}
	});
	//validator
	validator.create_item();
	validator.create_item();
	validator.item(0)->function(function_t);
	validator.item(1)->function(function_u);
	validator.item(0)->load_numeric("Test/data/Rigid 2D/Linear Bar/data.txt", 9, 0);
	validator.item(1)->load_numeric("Test/data/Rigid 2D/Linear Bar/data.txt", 9, 6);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
}