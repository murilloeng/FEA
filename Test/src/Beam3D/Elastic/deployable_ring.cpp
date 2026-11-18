//std
#include <cmath>
#include <vector>

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
static const uint32_t n = 128;
static const double r = 1.20e+02;
static const double h = 6.00e+00;
static const double w = 6.00e-01;
static const double E = 2.00e+05;
static const double v = 3.00e-01;

//Reference: J-M. Battini, PhD Thesis (2002), pp. 118

void test::beam3D::elastic::deployable_ring(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//nodes
	for(uint32_t i = 0; i < n; i++)
	{
		const double t = 2 * M_PI * i / n;
		model.mesh()->create_node(r * cos(t), r * sin(t), 0);
	}
	//elements
	for(uint32_t i = 0; i < n; i++)
	{
		const double t = 2 * M_PI * i / n + M_PI / n;
		model.mesh()->create_element(fea::mesh::elements::Type::Beam3D, {i, (i + 1) % n});
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->major_axis() = {cos(t), sin(t), 0};
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
	model.boundary()->create_support(n / 2, dof::Rotation_2);
	model.boundary()->create_support(n / 2, dof::Rotation_3);
	model.boundary()->create_support(n / 2, dof::Translation_2);
	model.boundary()->create_support(n / 2, dof::Translation_3);
	//loads
	section.width(w);
	section.height(h);
	section.compute();
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(n / 2, dof::Rotation_1, -1);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(2000);
	model.analysis()->solver_static_nonlinear()->step_adapt(true);
	model.analysis()->solver_static_nonlinear()->step_size(5.00e+01);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(n / 2);
	model.analysis()->solver_static_nonlinear()->iteration_desired(4);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Rotation_1);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::MinimalNorm);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Elastic/Deployable Ring");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Elastic/Deployable Ring/data.txt", {
		{n / 2, dof::Rotation_1}
	});
	//rotation (recorded in [-pi, pi], unwrapped)
	const uint32_t ns = model.analysis()->solver_static_nonlinear()->step();
	const uint32_t id = model.mesh()->node(n / 2)->dof_index(dof::Rotation_1);
	std::vector<double> t(ns), p(ns);
	for(uint32_t i = 0; i < ns; i++)
	{
		t[i] = -model.analysis()->solver_static_nonlinear()->state_data(i, id);
		p[i] = model.analysis()->solver_static_nonlinear()->load_data(i);
		if(i != 0) t[i] += 2 * M_PI * round((t[i - 1] - t[i]) / 2 / M_PI);
	}
	//validator
	validator.create_item();
	validator.item(0)->tolerance(3.50e-02);
	validator.item(0)->adjuster_reference([](double&, double& x2){ x2 *= 100; });
	validator.item(0)->load_numeric(t.data(), p.data(), ns);
	validator.item(0)->load_reference("Test/data/Beam 3D/Elastic/Deployable Ring/reference-beam.dat", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Deployable Ring/plot.gp");
}
