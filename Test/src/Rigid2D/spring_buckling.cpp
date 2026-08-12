//std
#include <cmath>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Joints/Type.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Nodal/Nodal.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Rigid2D.hpp"

//data
static const double a = 1.00e-02;
static const double L = 1.00e+00;
static const double K = 1.00e+03;
static const double P = 1.00e+03;

void test::rigid2D::spring_buckling(void)
{
	//data
	fea::Model model;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	typedef fea::mesh::joints::Type joint;
	typedef fea::mesh::elements::Type element;
	//nodes
	model.mesh()->create_node(0, 0, 0);
	model.mesh()->create_node(L * cos(a), L * sin(a), 0);
	//joints
	model.mesh()->create_joint(joint::Rigid2D, {0, 1});
	//elements
	model.mesh()->create_element(element::Nodal, { 0 });
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->stiffness(K);
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->dof(dof::Rotation_3);
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Translation_1, -P);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(1000);
	model.analysis()->solver_static_nonlinear()->attempt_max(1);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_1);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Rigid 2D/Spring Buckling");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Rigid 2D/Spring Buckling/data.txt", {
		{0, dof::Rotation_3}, {1, dof::Translation_1}, {1, dof::Translation_2}
	});
	//draw
	fea::draw::Engine(&model).start();
}