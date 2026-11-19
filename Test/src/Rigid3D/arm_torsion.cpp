//std
#include <cmath>

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Validation/Validator.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Joints/Type.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Rigid3D.hpp"

//data
static const uint32_t ne = 10;
static const double b = 1.00e-01;
static const double h = 1.00e-01;
static const double L = 1.00e+00;
static const double d = 5.00e-01;
static const double v = 3.00e-01;
static const double E = 2.10e+11;

static double function_u2(double t)
{
	return d * (cos(2 * M_PI * t) - 1);
}
static double function_u3(double t)
{
	return d * sin(2 * M_PI * t);
}

void test::rigid3D::arm_torsion(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::mesh::joints::Type joint;
	typedef fea::mesh::elements::Type element;
	//nodes
	for(uint32_t i = 0; i <= ne; i++)
	{
		model.mesh()->create_node(L * i / ne, 0, 0);
	}
	model.mesh()->create_node(L, d, 0);
	//joints
	model.mesh()->create_joint(joint::Rigid3D, {ne, ne + 1});
	//elements
	for(uint32_t i = 0; i < ne; i++)
	{
		model.mesh()->create_element(element::Beam3D, {i + 0, i + 1});
	}
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam3D*) element)->section(&section);
		((fea::mesh::elements::Beam3D*) element)->material(&material);
	}
	//supports
	model.boundary()->create_support(0, dof::Rotation_1);
	model.boundary()->create_support(0, dof::Rotation_2);
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(0, dof::Translation_3);
	//loads
	section.width(b);
	section.height(h);
	section.compute();
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	const double J = section.torsion_constant();
	const double G = material.shear_modulus();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(ne + 1, dof::Rotation_1, 2 * M_PI * G * J / L);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(100);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->convergence().type_add(math::solvers::Convergence::Type::State);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne + 1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Rotation_1);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Rigid 3D/Arm Torsion");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Rigid 3D/Arm Torsion/data.txt", {
		{ne + 1, dof::Translation_2}, {ne + 1, dof::Translation_3}
	});
	//validator
	validator.create_item();
	validator.create_item();
	validator.item(0)->function(function_u2);
	validator.item(1)->function(function_u3);
	validator.item(0)->load_numeric("Test/data/Rigid 3D/Arm Torsion/data.txt", 2, 0);
	validator.item(1)->load_numeric("Test/data/Rigid 3D/Arm Torsion/data.txt", 2, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Rigid 3D/Arm Torsion/plot.gp");
}