//std
#include <cmath>
#include <vector>

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
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t ne = 40;
static const double L = 2.40e-01;
static const double q = 4.00e-04;
static const double v = 3.10e-01;
static const double E = 7.12e+10;
static const double A = 1.00e-06;
static const double T = 2.50e-01;
static const double J = 2.16e-12;
static const double I = 8.33e-14;

//Reference: V. L. Corvec, PhD Thesis (2012), pp. 121 (data: B. Nour-Omid, C. C. Rankin, 1991)

void test::beam3D::elastic::hockling_cable(void)
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
		model.mesh()->create_node(L * i / ne, q * i / ne, 0);
	}
	//elements
	for(uint32_t i = 0; i < ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam3D, {i, i + 1});
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
	model.boundary()->create_support(ne, dof::Rotation_2);
	model.boundary()->create_support(ne, dof::Rotation_3);
	model.boundary()->create_support(ne, dof::Translation_2);
	model.boundary()->create_support(ne, dof::Translation_3);
	//loads
	section.area(A);
	section.inertia(0, I);
	section.inertia(1, I);
	section.torsion_constant(J);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(ne, dof::Rotation_1, T);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(1000);
	model.analysis()->solver_static_nonlinear()->step_adapt(true);
	model.analysis()->solver_static_nonlinear()->step_scale_max(4.00e+00);
	model.analysis()->solver_static_nonlinear()->step_size(1.00e-02);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne);
	model.analysis()->solver_static_nonlinear()->iteration_desired(4);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Rotation_1);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_min(-6.00e-01);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::MinimalNorm);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMinimum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Elastic/Hockling Cable");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Elastic/Hockling Cable/data.txt", {
		{ne, dof::Translation_1}, {ne, dof::Rotation_1}
	});
	//rotation (recorded in [-pi, pi], unwrapped)
	const uint32_t ns = model.analysis()->solver_static_nonlinear()->step();
	const uint32_t id = model.mesh()->node(ne)->dof_index(dof::Rotation_1);
	std::vector<double> t(ns), p(ns);
	for(uint32_t i = 0; i < ns; i++)
	{
		t[i] = model.analysis()->solver_static_nonlinear()->state_data(i, id);
		p[i] = model.analysis()->solver_static_nonlinear()->load_data(i);
		if(i != 0) t[i] += 2 * M_PI * round((t[i - 1] - t[i]) / 2 / M_PI);
	}
	//validator
	validator.create_item();
	validator.item(0)->tolerance(3.50e-02);
	validator.item(0)->adjuster_reference([](double&, double& x2){ x2 /= 1e3 * T; });
	validator.item(0)->load_numeric(t.data(), p.data(), ns);
	validator.item(0)->load_reference("Test/data/Beam 3D/Elastic/Hockling Cable/reference.dat", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Hockling Cable/plot.gp");
}
