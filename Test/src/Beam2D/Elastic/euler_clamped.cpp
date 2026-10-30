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
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t ne = 20;
static const double A = 1.00e+01;
static const double I = 1.00e-05;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double P = 4 * M_PI * M_PI * E * I / L / L;

//Reference: Timoshenko & Gere, Theory of Elastic Stability, sec. 2.7 (elastica)

void test::beam2D::elastic::euler_clamped(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//nodes
	for(uint32_t i = 0; i <= ne; i++)
	{
		model.mesh()->create_node(L * i / ne, 0, 0);
	}
	//elements
	section.area(A);
	section.inertia(1, I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	for(uint32_t i = 0; i < ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam2D, {i, i + 1});
		((fea::mesh::elements::Beam2D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Beam2D*) model.mesh()->element(i))->material(&material);
	}
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//supports
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(ne, dof::Rotation_3);
	model.boundary()->create_support(ne, dof::Translation_2);
	//loads
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->load_case(0)->create_load_node(ne, dof::Translation_1, -P);
	model.boundary()->load_case(0)->create_load_node(ne / 2, dof::Translation_2, +P * 1.00e-03);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(1000);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne / 2);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(3.00e+00);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Elastic/Euler Clamped");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Elastic/Euler Clamped/data.txt", {
		{ne, dof::Translation_1}, {ne / 2, dof::Translation_2}, {ne / 4, dof::Rotation_3}
	});
	//validator
	validator.create_item();
	validator.create_item();
	validator.create_item();
	validator.item(1)->adjuster_numeric([](double&, double& x2){ x2 = -x2; });
	validator.item(0)->load_numeric("Test/data/Beam 2D/Elastic/Euler Clamped/data.txt", 3, 1);
	validator.item(1)->load_numeric("Test/data/Beam 2D/Elastic/Euler Clamped/data.txt", 3, 0);
	validator.item(2)->load_numeric("Test/data/Beam 2D/Elastic/Euler Clamped/data.txt", 3, 2);
	validator.item(0)->load_reference("Test/data/Beam 2D/Elastic/Euler Clamped/reference.dat", 0, 1);
	validator.item(1)->load_reference("Test/data/Beam 2D/Elastic/Euler Clamped/reference.dat", 0, 2);
	validator.item(2)->load_reference("Test/data/Beam 2D/Elastic/Euler Clamped/reference.dat", 0, 3);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 2D/Elastic/Euler Clamped/plot.gp");
}
