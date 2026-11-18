//std
#include <cmath>

//Sections
#include "Sections/inc/Generic.hpp"

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
static const uint32_t ne = 60;
static const double L = 1.00e+01;
static const double A = 1.00e+04;
static const double I = 1.00e+02;
static const double E = 1.00e+00;
static const double v = -5.00e-01;
static const double P = 3.00e+01;
static const double M = 1.20e+02 * M_PI;

//Reference: J-M. Battini, PhD Thesis (2002), pp. 119

void test::beam3D::elastic::helix(void)
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
	for(uint32_t i = 0; i < ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam3D, {i, i + 1});
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->material(&material);
	}
	fea::mesh::elements::Beam::shear(true);
	//supports
	model.boundary()->create_support(0, dof::Rotation_1);
	model.boundary()->create_support(0, dof::Rotation_2);
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(0, dof::Translation_3);
	//loads
	section.area(A);
	section.inertia(0, I);
	section.inertia(1, I);
	section.shear_area(0, A);
	section.shear_area(1, A);
	section.torsion_constant(I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->load_case(0)->create_load_node(ne, dof::Rotation_3, M);
	model.boundary()->load_case(0)->create_load_node(ne, dof::Translation_3, P);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->step_adapt(true);
	model.analysis()->solver_static_nonlinear()->iteration_desired(4);
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(150);
	model.analysis()->solver_static_nonlinear()->step_size(1.00e-03);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_3);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(1.00e+00);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::LoadControl);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Elastic/Helix");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Elastic/Helix/data.txt", {
		{ne, dof::Translation_1}, {ne, dof::Translation_2}, {ne, dof::Translation_3}
	});
	//validator
	validator.create_item();
	validator.item(0)->tolerance(3.50e-02);
	validator.item(0)->adjuster_reference([](double&, double& x2){ x2 /= 6; });
	validator.item(0)->load_numeric("Test/data/Beam 3D/Elastic/Helix/data.txt", 2, 3);
	validator.item(0)->load_reference("Test/data/Beam 3D/Elastic/Helix/reference.dat", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Helix/plot.gp");
	fea::mesh::elements::Beam::shear(false);
}
