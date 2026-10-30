//std
#include <cmath>
#include <cstdio>

//Sections
#include "Sections/inc/Generic.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Solvers/Continuation.hpp"

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
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t ne = 40;
static const double A = 1.00e+02;
static const double I = 1.00e+00;
static const double R = 1.00e+02;
static const double v = 3.00e-01;
static const double E = 1.00e+06;
static const double b = 215 * M_PI / 180;

static const double p_ref = 8.97;
static const double tolerance = 2.00e-02;

//Reference: doi.org/10.1115/1.3423734
//Limit load: P R^2 / EI = 8.97 (inextensible elastica)

void test::beam2D::elastic::deep_arch(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	typedef fea::mesh::nodes::DOF dof;
	typedef math::solvers::Continuation::Type continuation;
	typedef math::solvers::StopCriteria::Type stop_criteria;
	//nodes
	for(uint32_t i = 0; i <= ne; i++)
	{
		const double t = -b / 2 + b * i / ne;
		model.mesh()->create_node(R * sin(t), R * cos(t), 0);
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
	model.boundary()->create_support(ne, dof::Translation_1);
	model.boundary()->create_support(ne, dof::Translation_2);
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(ne / 2, dof::Translation_2, -E * I / R / R);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(2000);
	model.analysis()->solver_static_nonlinear()->step_size(1.00e+01);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne / 2);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(10);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->continuation().type(continuation::ArcLengthCylindrical);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(stop_criteria::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Elastic/Deep Arch");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Elastic/Deep Arch/data.txt", {
		{ne / 2, dof::Translation_1}, {ne / 2, dof::Translation_2}
	});
	//limit load
	double p_num = 0;
	const double* p = model.analysis()->solver_static_nonlinear()->load_data();
	for(uint32_t i = 1; i < model.analysis()->solver_static_nonlinear()->step_max(); i++)
	{
		if(p[i] > p[i - 1] && p[i] > p[i + 1])
		{
			p_num = p[i];
			break;
		}
	}
	//validate
	const double error = fabs(p_num - p_ref) / p_ref;
	printf("Status: %s\n", error < tolerance ? "✅" : "❌");
	printf("Numeric %.2f Reference: %.2f Error: %.2e\n", p_num, p_ref, error);
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 2D/Elastic/Deep Arch/plot.gp");
}
