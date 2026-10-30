//std
#include <cmath>
#include <cstdio>
#include <vector>

//Sections
#include "Sections/inc/Generic.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t ne = 10;
static const uint32_t ns = 100;
static const double A = 1.00e+01;
static const double I = 1.00e-05;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double tolerance = 1.00e-06;
static const double c[] = {3.00e-01, 2.00e-01};

//Reference: a rigid motion stores no energy and produces no internal forces (patch test); the tolerance
//allows for round-off in the axial force, amplified by E A L^2 / E I = 1e6

void test::beam2D::elastic::rigid_rotation(void)
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
	//elements
	for(uint32_t i = 0; i < ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam2D, {i, i + 1});
		((fea::mesh::elements::Beam2D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Beam2D*) model.mesh()->element(i))->material(&material);
	}
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//section
	section.area(A);
	section.inertia(1, I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.check();
	model.setup();
	//data
	double U_max = 0, f_max = 0;
	double* x = model.analysis()->solver()->state_new();
	const uint32_t nu = model.analysis()->assembler()->dof_unknow();
	FILE* file = fopen("Test/data/Beam 2D/Elastic/Rigid Rotation/data.txt", "w");
	//rotation
	std::vector<double> f(nu);
	for(uint32_t s = 0; s <= ns; s++)
	{
		//state: rigid rotation about the point c, through a full turn (passes +/- pi at s = ns / 2)
		const double t = 2 * M_PI * s / ns;
		for(const fea::mesh::nodes::Node* node : model.mesh()->nodes())
		{
			const double* z = node->position();
			const double r1 = z[0] - c[0], r2 = z[1] - c[1];
			x[node->dof_index(dof::Translation_1)] = c[0] + cos(t) * r1 - sin(t) * r2 - z[0];
			x[node->dof_index(dof::Translation_2)] = c[1] + sin(t) * r1 + cos(t) * r2 - z[1];
			x[node->dof_index(dof::Rotation_3)] = t;
		}
		//compute
		double U = 0, fn = 0;
		model.compute();
		model.analysis()->assembler()->assemble_internal_energy(U);
		model.analysis()->assembler()->assemble_internal_force(f.data());
		for(uint32_t i = 0; i < nu; i++) fn += f[i] * f[i];
		//normalize
		U = fabs(U) / (E * I / L);
		fn = sqrt(fn) / (E * I / L / L);
		U_max = fmax(U_max, U);
		f_max = fmax(f_max, fn);
		fprintf(file, "%+.6e %+.6e %+.6e\n", t, U, fn);
		//update
		model.update();
	}
	fclose(file);
	//validate
	printf("Status: %s\n", U_max < tolerance && f_max < tolerance ? "✅" : "❌");
	printf("Rigid Rotation: max U / (EI / L) = %+.2e max |f| / (EI / L^2) = %+.2e\n", U_max, f_max);
	//draw
	fea::draw::Engine::plot("Test/data/Beam 2D/Elastic/Rigid Rotation/plot.gp");
}
