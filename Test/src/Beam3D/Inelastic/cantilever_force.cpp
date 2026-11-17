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
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t nw = 10;
static const uint32_t nh = 40;
static const uint32_t ne = 40;
static const double w = 2.00e-01;
static const double h = 4.00e-01;
static const double L = 3.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double K = 0.00e+00;
static const double sy = 2.50e+08;

void test::beam3D::inelastic::cantilever_force(void)
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
	model.mesh()->create_node(L, 0, 0);
	for(uint32_t k = 1; k < ne; k++)
	{
		model.mesh()->create_node(L * k / ne, 0, 0);
	}
	//elements
	for(uint32_t k = 0; k < ne; k++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam3D, {k == 0 ? 0 : k + 1, k + 1 == ne ? 1 : k + 2});
	}
	//elements
	section.width(w);
	section.height(h);
	section.fibers_width(nw);
	section.fibers_height(nh);
	material.yield_stress(sy);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	material.plastic_modulus(K);
	materials::Mechanic::inelastic(true);
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam3D*) element)->section(&section);
		((fea::mesh::elements::Beam3D*) element)->material(&material);
	}
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//supports
	model.boundary()->create_support(0, dof::Rotation_1);
	model.boundary()->create_support(0, dof::Rotation_2);
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(0, dof::Translation_3);
	//loads
	section.compute();
	const double W = section.plastic_modulus(1);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Translation_2, -sy * W / L);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->stop_criteria().state_min(-6.00e-02);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::ArcLengthCylindrical);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::StateLimitMinimum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Inelastic/Cantilever Force");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Inelastic/Cantilever Force/data.txt", {
		{1, dof::Translation_1}, {1, dof::Translation_2}
	});
	//validator
	validator.create_item();
	validator.item(0)->load_numeric("Test/data/Beam 3D/Inelastic/Cantilever Force/data.txt", 2, 1);
	validator.item(0)->load_reference("Test/data/Beam 3D/Inelastic/Cantilever Force/reference.dat", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Inelastic/Cantilever Force/plot.gp");
}