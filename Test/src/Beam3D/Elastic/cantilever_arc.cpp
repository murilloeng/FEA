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

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t ne = 8;
static const double w = 1.00e-01;
static const double h = 1.00e-01;
static const double R = 1.00e+01;
static const double v = 0.00e+00;
static const double E = 1.00e+07;
static const double P = 5.00e+01;

//Reference: P. Carotenuto, PhD Thesis (2006), pp. 95

void test::beam3D::elastic::cantilever_arc(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//nodes
	for(uint32_t i = 0; i <= ne; i++)
	{
		const double t = M_PI_4 * i / ne;
		model.mesh()->create_node(R * cos(t), R * sin(t), 0);
	}
	//elements
	for(uint32_t i = 0; i < ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam3D, {i, i + 1});
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->material(&material);
	}
	//supports
	model.boundary()->create_support(0, dof::Rotation_1);
	model.boundary()->create_support(0, dof::Rotation_2);
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(0, dof::Translation_3);
	//loads
	section.width(w);
	section.height(h);
	section.compute();
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(ne, dof::Translation_3, P);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(100);
	model.analysis()->solver_static_nonlinear()->step_size(1.00e-02);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_3);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(1.00e+00);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::LoadControl);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Elastic/Cantilever Arc");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Elastic/Cantilever Arc/data.txt", {
		{ne, dof::Translation_1}, {ne, dof::Translation_2}, {ne, dof::Translation_3}
	});
	//validator
	validator.create_item();
	validator.create_item();
	validator.create_item();
	validator.item(0)->tolerance(3.50e-02);
	validator.item(1)->tolerance(3.50e-02);
	validator.item(2)->tolerance(3.50e-02);
	validator.item(1)->adjuster_reference([](double&, double& x2){ x2 /= P; });
	validator.item(2)->adjuster_reference([](double&, double& x2){ x2 /= P; });
	validator.item(0)->adjuster_reference([](double& x1, double& x2){ x1 = -x1; x2 /= P; });
	validator.item(0)->load_numeric("Test/data/Beam 3D/Elastic/Cantilever Arc/data.txt", 0, 3);
	validator.item(1)->load_numeric("Test/data/Beam 3D/Elastic/Cantilever Arc/data.txt", 1, 3);
	validator.item(2)->load_numeric("Test/data/Beam 3D/Elastic/Cantilever Arc/data.txt", 2, 3);
	validator.item(0)->load_reference("Test/data/Beam 3D/Elastic/Cantilever Arc/reference-u.dat", 0, 1);
	validator.item(1)->load_reference("Test/data/Beam 3D/Elastic/Cantilever Arc/reference-v.dat", 0, 1);
	validator.item(2)->load_reference("Test/data/Beam 3D/Elastic/Cantilever Arc/reference-w.dat", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Cantilever Arc/plot.gp");
}