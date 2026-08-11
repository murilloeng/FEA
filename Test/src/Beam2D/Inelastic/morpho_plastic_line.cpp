//std
#include <cmath>

//Math
#include "Math/inc/Miscellaneous/util.hpp"

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Joints/Type.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Nodal/Nodal.hpp"

#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data

//reference: doi.org/10.48550/arXiv.2412.06022

void test::beam2D::inelastic::morpho_plastic_line(void)
{
	//data
	fea::Model model;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	//nodes
	model.mesh()->create_node(0, 0, 0);
	model.mesh()->create_node(1, 0, 0);
	//joints
	model.mesh()->create_joint(fea::mesh::joints::Type::Rigid2D, {0, 1});
	//elements
	model.mesh()->create_element(fea::mesh::elements::Type::Nodal, { 0 });
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->stiffness(1e3);
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->dof(dof::Rotation_3);
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(0, dof::Rotation_3, 1e3);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(1000);
	model.analysis()->solver_static_nonlinear()->attempt_max(1);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_1);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::LoadControl);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Inelastic/MorphoPlastic Line");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Inelastic/MorphoPlastic Line/data.txt", {
		{1, dof::Translation_1}, {1, dof::Translation_2}
	});
	//draw
	fea::draw::Engine(&model).start();
}