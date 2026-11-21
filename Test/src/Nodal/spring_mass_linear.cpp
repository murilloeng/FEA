//std
#include <cmath>

//Math
#include "Math/inc/Validation/Validator.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Nodal/Nodal.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicLinear.hpp"

//Test
#include "FEA/Test/inc/Nodal.hpp"

//data
static const double np = 5;
static const double M = 1.00e+00;
static const double P = 1.00e+00;
static const double K = 4 * M_PI * M_PI;

static double function_u(double t)
{
	return P / K * (1 - cos(sqrt(K / M) * t));
}

void test::nodal::spring_mass_linear(void)
{
	//data
	fea::Model model;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::mesh::elements::Type element;
	//nodes
	model.mesh()->create_node(0, 0, 0);
	//elements
	model.mesh()->create_element(element::Nodal, { 0 });
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->inertia(M);
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->stiffness(K);
	((fea::mesh::elements::Nodal*) model.mesh()->element(0))->dof(dof::Translation_1);
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(0, dof::Translation_1, P);
	//setup
	model.analysis()->solver_dynamic_linear()->enable();
	model.analysis()->solver_dynamic_linear()->silent(true);
	model.analysis()->solver_dynamic_linear()->step_max(1000);
	model.analysis()->solver_dynamic_linear()->load_combination(0);
	model.analysis()->solver_dynamic_linear()->time_max(np * 2 * M_PI * sqrt(M / K));
	model.analysis()->solver_dynamic_linear()->watch_dof().node(0);
	model.analysis()->solver_dynamic_linear()->watch_dof().dof(dof::Translation_1);
	//solve
	model.solve();
	//save
	model.analysis()->solver_dynamic_linear()->save("Test/data/Nodal/Spring Mass Linear/data.txt", {
		{0, dof::Translation_1}
	});
	//validator
	validator.create_item();
	validator.item(0)->function(function_u);
	validator.item(0)->load_numeric("Test/data/Nodal/Spring Mass Linear/data.txt", 3, 0);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
}