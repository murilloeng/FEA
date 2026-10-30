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
static const double R = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double pc = 6.00e+00;
static const double pt = 2.00e+01;

//reference: inextensible elastica of the ring (Test/data/Beam 2D/Elastic/Pinched Ring/reference.py)
//two rings in one model, the first pushed in (compression) and the second pulled out (tension), with
//P R^2 / EI = pc and pt at load factor 1; each ring is a quarter model between the side point B (on the
//horizontal axis) and the loaded point A (on the vertical axis): both keep their slope (symmetry), B cannot
//move vertically and A cannot move horizontally, and A carries half of the load P

void test::beam2D::elastic::pinched_ring(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//nodes
	for(uint32_t k = 0; k < 2; k++)
	{
		for(uint32_t i = 0; i <= ne; i++)
		{
			const double t = M_PI_2 * i / ne;
			model.mesh()->create_node(R * cos(t), R * sin(t), 0);
		}
	}
	//elements
	for(uint32_t k = 0; k < 2; k++)
	{
		for(uint32_t i = 0; i < ne; i++)
		{
			const uint32_t n = k * (ne + 1) + i;
			model.mesh()->create_element(fea::mesh::elements::Type::Beam2D, {n, n + 1});
		}
	}
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam2D*) element)->section(&section);
		((fea::mesh::elements::Beam2D*) element)->material(&material);
	}
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//supports
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(ne, dof::Rotation_3);
	model.boundary()->create_support(ne, dof::Translation_1);
	model.boundary()->create_support(ne + 1, dof::Rotation_3);
	model.boundary()->create_support(ne + 1, dof::Translation_2);
	model.boundary()->create_support(2 * ne + 1, dof::Rotation_3);
	model.boundary()->create_support(2 * ne + 1, dof::Translation_1);
	//loads
	section.area(A);
	section.inertia(1, I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->load_case(0)->create_load_node(ne, dof::Translation_2, -pc * E * I / R / R / 2);
	model.boundary()->load_case(0)->create_load_node(2 * ne + 1, dof::Translation_2, +pt * E * I / R / R / 2);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(1000);
	model.analysis()->solver_static_nonlinear()->step_size(2.50e-03);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(1.00e+00);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Elastic/Pinched Ring");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Elastic/Pinched Ring/data-compression.txt", {
		{ne, dof::Translation_2}, {0, dof::Translation_1}
	});
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Elastic/Pinched Ring/data-tension.txt", {
		{2 * ne + 1, dof::Translation_2}, {ne + 1, dof::Translation_1}
	});
	//validator
	validator.create_item();
	validator.create_item();
	validator.create_item();
	validator.create_item();
	validator.item(0)->adjuster_numeric([](double& p, double& x){ p *= pc; x = -x / R; });
	validator.item(1)->adjuster_numeric([](double& p, double& x){ p *= pc; x = +x / R; });
	validator.item(2)->adjuster_numeric([](double& p, double& x){ p *= pt; x = +x / R; });
	validator.item(3)->adjuster_numeric([](double& p, double& x){ p *= pt; x = -x / R; });
	validator.item(2)->load_numeric("Test/data/Beam 2D/Elastic/Pinched Ring/data-tension.txt", 2, 0);
	validator.item(3)->load_numeric("Test/data/Beam 2D/Elastic/Pinched Ring/data-tension.txt", 2, 1);
	validator.item(0)->load_numeric("Test/data/Beam 2D/Elastic/Pinched Ring/data-compression.txt", 2, 0);
	validator.item(1)->load_numeric("Test/data/Beam 2D/Elastic/Pinched Ring/data-compression.txt", 2, 1);
	validator.item(2)->load_reference("Test/data/Beam 2D/Elastic/Pinched Ring/reference-tension.dat", 0, 1);
	validator.item(3)->load_reference("Test/data/Beam 2D/Elastic/Pinched Ring/reference-tension.dat", 0, 2);
	validator.item(0)->load_reference("Test/data/Beam 2D/Elastic/Pinched Ring/reference-compression.dat", 0, 1);
	validator.item(1)->load_reference("Test/data/Beam 2D/Elastic/Pinched Ring/reference-compression.dat", 0, 2);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 2D/Elastic/Pinched Ring/plot.gp");
}
