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
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"

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
	sections::Rectangle section;
	materials::Uniaxial material;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	//points
	model.geometry()->create_point(0, 0, 0);
	model.geometry()->create_point(1, 0, 0);
	//curves
	model.geometry()->create_line(0, 1);
	model.geometry()->curve(0)->structured(1);
	model.geometry()->curve(0)->element_type(fea::mesh::elements::Type::Truss2D);
	//generate
	model.geometry()->generate_mesh();
	//elements
	section.width(0.1);
	section.height(0.1);
	section.compute();
	material.poisson_ratio(3.00e-01);
	material.elastic_modulus(2.10e+11);
	((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->section(&section);
	((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->material(&material);
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//constraints
	model.boundary()->create_constraint({ 1, 1 }, { dof::Translation_1, dof::Translation_2 });
	model.boundary()->constraint(0)->function([](double& f, const double* d){
		f = d[1] - d[0] * d[0];
	});
	model.boundary()->constraint(0)->gradient([](double* g, const double* d){
		g[0] = -2 * d[0];
		g[1] = +1;
	});
	model.boundary()->constraint(0)->hessian([](double* H, const double* d){
		H[0 + 2 * 0] = -2;
		H[1 + 2 * 0] = 0;
		H[0 + 2 * 1] = 0;
		H[1 + 2 * 1] = 0;
	});
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Translation_1, 1e8);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(1000);
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