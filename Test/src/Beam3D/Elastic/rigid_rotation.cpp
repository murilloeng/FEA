//std
#include <cmath>
#include <cstdio>

//Sections
#include "Sections/inc/Generic.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Groups/ASO3.hpp"
#include "Math/inc/Groups/GSO3.hpp"
#include "Math/inc/Linear/Matrix.hpp"
#include "Math/inc/Solvers/Convergence.hpp"
#include "Math/inc/Solvers/Continuation.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t ne = 10;
static const uint32_t ns = 1000;
static const double A = 1.00e+02;
static const double I = 1.00e+00;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 1.00e+03;
static const double tolerance = 1.00e-08;

//Reference: a rigid motion stores no energy (patch test); an L-shaped frame is driven only through the
//translations of three non-collinear nodes along a genuinely 3D rigid motion (two rotations about skew axes,
//about 5.5 turns in total, at a non-uniform rate); every node must follow the rigid motion with zero energy
//(shear deformable: with the shear penalty of the Bernoulli option Newton does not converge from the first step)

static math::Mat3 rotation(double t)
{
	//rates (non-uniform steps)
	const double a = 4 * M_PI * (t + 0.15 * sin(2 * M_PI * t));
	const double b = 3 * M_PI * t * t;
	//axes
	const math::Vec3 n1 = math::Vec3(1, 2, 3).unit();
	const math::Vec3 n2 = math::Vec3(-2, 1, 1).unit();
	//rotation
	return math::ASO3(a * n1).exp().matrix() * math::ASO3(b * n2).exp().matrix();
}
static math::Vec3 position(const math::Vec3& z, double t)
{
	const math::Vec3 c(3.00e-01, -2.00e-01, 1.00e-01);
	const math::Vec3 d(5.00e-01 * t, 2.00e-01 * sin(M_PI * t), -3.00e-01 * t * t);
	return c + d + rotation(t) * (z - c);
}

void test::beam3D::elastic::rigid_rotation(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//nodes
	for(uint32_t i = 0; i <= ne; i++)
	{
		model.mesh()->create_node(L * i / ne, 0, 0);
	}
	for(uint32_t i = 1; i <= ne; i++)
	{
		model.mesh()->create_node(L, 0, L * i / ne);
	}
	//elements
	for(uint32_t i = 0; i < 2 * ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam3D, {i, i + 1});
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->material(&material);
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->major_axis() = {0, 1, 0};
	}
	fea::mesh::elements::Beam::shear(true);
	//supports
	const uint32_t nodes[] = {0, ne, 2 * ne};
	const dof dofs[] = {dof::Translation_1, dof::Translation_2, dof::Translation_3};
	for(uint32_t node : nodes)
	{
		const math::Vec3 z(model.mesh()->node(node)->position());
		for(uint32_t i = 0; i < 3; i++)
		{
			model.boundary()->create_support(node, dofs[i]);
			model.boundary()->supports().back()->state([z, i](double t){ return position(z, t)[i] - z[i]; });
		}
	}
	//section
	section.area(A);
	section.inertia(0, I);
	section.inertia(1, 2 * I);
	section.shear_area(0, A);
	section.shear_area(1, A);
	section.torsion_constant(I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(ns);
	model.analysis()->solver_static_nonlinear()->convergence().tolerance(1.00e-08);
	model.analysis()->solver_static_nonlinear()->convergence().type_set(math::solvers::Convergence::Type::State);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::LoadControl);
	//solve
	model.solve();
	//check
	double U_max = 0, u_max = 0, r_max = 0;
	const fea::analysis::StaticNonlinear* solver = model.analysis()->solver_static_nonlinear();
	FILE* file = fopen("Test/data/Beam 3D/Elastic/Rigid Rotation/data.txt", "w");
	for(uint32_t s = 0; s < solver->step(); s++)
	{
		//data
		double u_step = 0, r_step = 0;
		const double t = solver->time_data(s);
		const double U = fabs(solver->internal_energy_data(s)) / (E * I / L);
		//nodes
		for(const fea::mesh::nodes::Node* node : model.mesh()->nodes())
		{
			//data
			math::Vec3 x, r;
			const math::Vec3 z(node->position());
			const math::Vec3 p = position(z, t);
			//state
			for(uint32_t i = 0; i < 3; i++)
			{
				const uint32_t iu = node->dof_index(dof(i));
				const uint32_t ir = node->dof_index(dof(i + 3));
				x[i] = z[i] + (iu < model.analysis()->assembler()->dof_unknow() ? solver->state_data(s, iu) : position(z, t)[i] - z[i]);
				r[i] = solver->state_data(s, ir);
			}
			//errors
			u_step = fmax(u_step, (x - p).norm() / L);
			r_step = fmax(r_step, (math::ASO3(r).exp().matrix() - rotation(t)).norm());
		}
		fprintf(file, "%+.6e %+.6e %+.6e %+.6e\n", t, U, u_step, r_step);
		U_max = fmax(U_max, U);
		u_max = fmax(u_max, u_step);
		r_max = fmax(r_max, r_step);
	}
	fclose(file);
	fea::mesh::elements::Beam::shear(false);
	//validate
	const bool complete = solver->step() == ns + 1;
	printf("Status: %s\n", complete && U_max < tolerance && u_max < tolerance && r_max < tolerance ? "✅" : "❌");
	printf("Rigid Rotation: max U / (EI / L) = %+.2e max |x - x_rigid| / L = %+.2e max |R - R_rigid| = %+.2e\n", U_max, u_max, r_max);
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Rigid Rotation/plot.gp");
}
