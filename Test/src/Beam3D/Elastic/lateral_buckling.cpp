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
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t ne = 40;
static const uint32_t ns = 200;
static const double A = 1.00e+04;
static const double I = 1.00e+00;
static const double J = 2.60e+00;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 1.00e+00;
static const double r = 1.00e+03;
static const double q = 1.00e-04;

//Reference: S. P. Timoshenko and J. M. Gere, Theory of Elastic Stability (1961), sec. 6.4; lateral buckling of a
//cantilever with a tip force at the centroid (no warping): P_cr = 4.013 sqrt(E I2 G J) / L^2 (pre-buckling
//deflection neglected, I3 / I2 = 1000 here); the critical load is estimated from the imperfect path (small
//lateral tip force) with a Southwell fit

void test::beam3D::elastic::lateral_buckling(void)
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
	//section
	section.area(A);
	section.inertia(0, I);
	section.inertia(1, r * I);
	section.torsion_constant(J);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	//loads
	const double G = E / 2 / (1 + v);
	const double Pc = 4.013 * sqrt(E * I * G * J) / L / L;
	model.boundary()->create_load_case(ne, dof::Translation_2, -Pc);
	model.boundary()->create_load_case(ne, dof::Translation_3, q * Pc);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->load_combination(0)->create_load_item(1, 1, true);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(ns);
	model.analysis()->solver_static_nonlinear()->step_size(9.70e-01 / ns);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_3);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::LoadControl);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Elastic/Lateral Buckling");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Elastic/Lateral Buckling/data.txt", {
		{ne, dof::Translation_2}, {ne, dof::Translation_3}, {ne, dof::Rotation_1}
	});
	//southwell: w = w0 + w * P / P_cr
	double s1 = 0, s2 = 0, s11 = 0, s12 = 0, n = 0;
	const fea::analysis::StaticNonlinear* solver = model.analysis()->solver_static_nonlinear();
	const uint32_t i3 = model.mesh()->node(ne)->dof_index(dof::Translation_3);
	for(uint32_t s = 0; s < solver->step(); s++)
	{
		const double p = solver->load_data(s);
		const double w = solver->state_data(s, i3);
		if(p < 0.9) continue;
		n += 1;
		s1 += w * p;
		s2 += w;
		s11 += w * p * w * p;
		s12 += w * p * w;
	}
	const double pn = (n * s11 - s1 * s1) / (n * s12 - s1 * s2);
	//validate
	const bool test = solver->step() == ns + 1 && fabs(pn - 1) < 1.00e-02;
	printf("Status: %s\n", test ? "✅" : "❌");
	printf("Lateral Buckling: P_cr (Southwell) / P_cr (Timoshenko) = %.6f (%u points)\n", pn, uint32_t(n));
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Lateral Buckling/plot.gp");
}
