//std
#include <cmath>

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
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t ne = 10;
static const double L = 1.20e+02;
static const double v = 3.00e-01;
static const double E = 7.20e+02;
static const double A = 6.00e+00;
static const double I = 2.00e+00;

//reference: doi.org/10.1061/JMCEA3.0000966

void test::beam2D::elastic::lee_frame(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	//nodes
	for(uint32_t i = 0; i <= ne; i++)
	{
		model.mesh()->create_node(0, L * i / ne, 0);
	}
	for(uint32_t i = 1; i <= ne; i++)
	{
		model.mesh()->create_node(L * i / ne, L, 0);
	}
	//elements
	for(uint32_t i = 0; i < 2 * ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam2D, {i, i + 1});
		((fea::mesh::elements::Beam2D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Beam2D*) model.mesh()->element(i))->material(&material);
	}
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(2 * ne, dof::Translation_1);
	model.boundary()->create_support(2 * ne, dof::Translation_2);
	//loads
	section.area(A);
	section.inertia(1, I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(12, dof::Translation_2, -1);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->step_size(1.00e-01);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(12);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->stop_criteria().parameter_max(3.00e+00);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::ParameterLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Lee Frame");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Lee Frame/data.txt", {
		{12, dof::Translation_1}, {12, dof::Translation_2}, {12, dof::Rotation_3}
	});
	//validator
	validator.create_item();
	validator.create_item();
	validator.item(0)->tolerance(1.20e-01);
	validator.item(1)->tolerance(1.20e-01);
	validator.item(0)->load_numeric("Test/data/Beam 2D/Lee Frame/data.txt", 0, 3);
	validator.item(1)->load_numeric("Test/data/Beam 2D/Lee Frame/data.txt", 1, 3);
	validator.item(0)->load_reference("Test/data/Beam 2D/Lee Frame/reference-u.dat", 0, 1);
	validator.item(1)->load_reference("Test/data/Beam 2D/Lee Frame/reference-v.dat", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
}