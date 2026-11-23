//std
#include <cmath>

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Solvers/Continuation.hpp"
#include "Math/inc/Validation/Validator.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t ne = 10;
static const double L = 2.40e+02;
static const double q = 4.00e-02;
static const double w = 6.00e-01;
static const double h = 3.00e+01;
static const double v = 3.10e-01;
static const double E = 7.12e+04;
static const double M = 6.267e+02;

//Reference: J-M. Battini, PhD Thesis (2002), pp. 120 (data: B. Nour-Omid, C. C. Rankin, 1991)

void test::beam3D::elastic::frame_right_angle(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//nodes
	model.mesh()->create_node(0, 0, 0);
	for(uint32_t i = 1; i <= ne; i++)
	{
		model.mesh()->create_node(-L * cos(M_PI_4) * i / ne, -L * sin(M_PI_4) * i / ne, q * i / ne);
	}
	for(uint32_t i = 1; i <= ne; i++)
	{
		model.mesh()->create_node(+L * cos(M_PI_4) * i / ne, -L * sin(M_PI_4) * i / ne, q * i / ne);
	}
	//elements
	for(uint32_t i = 0; i < 2 * ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam3D, {i == ne ? 0 : i, i + 1});
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->material(&material);
	}
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(ne, dof::Rotation_1);
	model.boundary()->create_support(ne, dof::Rotation_2);
	model.boundary()->create_support(ne, dof::Translation_2);
	model.boundary()->create_support(ne, dof::Translation_3);
	model.boundary()->create_support(2 * ne, dof::Rotation_1);
	model.boundary()->create_support(2 * ne, dof::Rotation_2);
	model.boundary()->create_support(2 * ne, dof::Translation_2);
	model.boundary()->create_support(2 * ne, dof::Translation_3);
	//loads
	section.width(w);
	section.height(h);
	section.compute();
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->load_case(0)->create_load_node(1 * ne, dof::Rotation_3, +M);
	model.boundary()->load_case(0)->create_load_node(2 * ne, dof::Rotation_3, -M);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(1000);
	model.analysis()->solver_static_nonlinear()->step_adapt(true);
	model.analysis()->solver_static_nonlinear()->step_size(9.00e-01);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(0);
	model.analysis()->solver_static_nonlinear()->iteration_desired(4);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_3);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_min(-9.80e-01);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::ArcLengthCylindrical);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMinimum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Elastic/Frame Right Angle");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Elastic/Frame Right Angle/data.txt", {
		{0, dof::Translation_2}, {0, dof::Translation_3}
	});
	//validator
	validator.create_item();
	validator.item(0)->tolerance(3.50e-02);
	validator.item(0)->adjuster_numeric([](double& x1, double&){ x1 = fabs(x1); });
	validator.item(0)->adjuster_reference([](double& x1, double& x2){ x1 = fabs(x1); x2 /= M; });
	validator.item(0)->load_numeric("Test/data/Beam 3D/Elastic/Frame Right Angle/data.txt", 1, 2);
	validator.item(0)->load_reference("Test/data/Beam 3D/Elastic/Frame Right Angle/reference.dat", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Frame Right Angle/plot.gp");
}
