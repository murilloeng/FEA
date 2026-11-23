//std
#include <cmath>
#include <cstdio>
#include <functional>

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
#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t ne = 10;
static const uint32_t ns = 300;
static const double A = 1.00e+02;
static const double I = 1.00e+00;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 1.00e+00;
static const double tolerance = 1.00e-08;

//Reference: conservative loads have a path independent response; a cantilever is loaded by two tip forces
//(P2 = 3 E I3 / L^2, P3 = 3 E I2 / L^2) applied one after the other in both orders and then removed together;
//the two paths must reach the same deformed state (large 3D rotations with bending-torsion coupling) and both
//must return to the undeformed state (G. Jelenić and M. A. Crisfield, CMAME 171, 1999)

static double ramp(double t, double a, double b)
{
	return fmin(fmax((t - a) / (b - a), 0), 1);
}
static void path(bool order, math::Vec3* u, math::Mat3* R, double* U, const char* path, const char* deformed)
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
	fea::mesh::elements::Beam::shear(true);
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
	section.inertia(1, 2 * I);
	section.shear_area(0, A);
	section.shear_area(1, A);
	section.torsion_constant(I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	//loads
	const double t1 = order ? 0 : 1. / 3, t2 = order ? 1. / 3 : 0;
	model.boundary()->create_load_case(ne, dof::Translation_2, 6 * E * I / L / L);
	model.boundary()->create_load_case(ne, dof::Translation_3, 3 * E * I / L / L);
	model.boundary()->load_case(0)->load_node(0)->time_function([t1](double t){ return ramp(t, t1, t1 + 1. / 3) - ramp(t, 2. / 3, 1); });
	model.boundary()->load_case(1)->load_node(0)->time_function([t2](double t){ return ramp(t, t2, t2 + 1. / 3) - ramp(t, 2. / 3, 1); });
	model.boundary()->create_load_combination(0, true, 1);
	model.boundary()->load_combination(0)->create_load_item(1, 1, true);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(ns);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->convergence().tolerance(1.00e-10);
	model.analysis()->solver_static_nonlinear()->convergence().type_set(math::solvers::Convergence::Type::State);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::LoadControl);
	//solve
	model.solve();
	fea::mesh::elements::Beam::shear(false);
	//save
	model.analysis()->solver_static_nonlinear()->save(path, {
		{ne, dof::Translation_1}, {ne, dof::Translation_2}, {ne, dof::Translation_3}
	});
	//state
	const fea::mesh::nodes::Node* node = model.mesh()->node(ne);
	const fea::analysis::StaticNonlinear* solver = model.analysis()->solver_static_nonlinear();
	for(uint32_t k = 0; k < 2; k++)
	{
		math::Vec3 r;
		const uint32_t s = k == 0 ? 2 * ns / 3 : ns;
		for(uint32_t i = 0; i < 3; i++)
		{
			u[k][i] = s < solver->step() ? solver->state_data(s, node->dof_index(dof(i + 0))) : NAN;
			r[i] = s < solver->step() ? solver->state_data(s, node->dof_index(dof(i + 3))) : NAN;
		}
		R[k] = math::ASO3(r).exp().matrix();
		U[k] = s < solver->step() ? solver->internal_energy_data(s) : NAN;
	}
	//deformed (loaded state)
	FILE* file = fopen(deformed, "w");
	const uint32_t nu = model.analysis()->assembler()->dof_unknow();
	for(const fea::mesh::nodes::Node* node : model.mesh()->nodes())
	{
		for(uint32_t i = 0; i < 3; i++)
		{
			const uint32_t index = node->dof_index(dof(i));
			const bool test = index < nu && 2 * ns / 3 < solver->step();
			fprintf(file, "%+.6e %+.6e ", node->position()[i], node->position()[i] + (test ? solver->state_data(2 * ns / 3, index) : 0));
		}
		fprintf(file, "\n");
	}
	fclose(file);
}

void test::beam3D::elastic::path_independence(void)
{
	//data
	double Ua[2], Ub[2];
	math::Vec3 ua[2], ub[2];
	math::Mat3 Ra[2], Rb[2];
	const math::Mat3 I3(math::Mode::eye);
	//paths
	path(true, ua, Ra, Ua, "Test/data/Beam 3D/Elastic/Path Independence/data-a.txt", "Test/data/Beam 3D/Elastic/Path Independence/deformed-a.txt");
	path(false, ub, Rb, Ub, "Test/data/Beam 3D/Elastic/Path Independence/data-b.txt", "Test/data/Beam 3D/Elastic/Path Independence/deformed-b.txt");
	//errors
	const double eu = (ua[0] - ub[0]).norm() / L;
	const double er = (Ra[0] - Rb[0]).norm();
	const double eU = fabs(Ua[0] - Ub[0]) / fabs(Ua[0]);
	const double zu = fmax(ua[1].norm(), ub[1].norm()) / L;
	const double zr = fmax((Ra[1] - I3).norm(), (Rb[1] - I3).norm());
	const double zU = fmax(fabs(Ua[1]), fabs(Ub[1])) / (E * I / L);
	//validate
	const bool test = eu < tolerance && er < tolerance && eU < tolerance && zu < tolerance && zr < tolerance && zU < tolerance;
	printf("Status: %s\n", test ? "✅" : "❌");
	printf("Path Independence: loaded |ua - ub| / L = %+.2e |Ra - Rb| = %+.2e |Ua - Ub| / U = %+.2e (tip: %+.4f %+.4f %+.4f)\n", eu, er, eU, ua[0][0], ua[0][1], ua[0][2]);
	printf("Path Independence: unloaded |u| / L = %+.2e |R - I| = %+.2e U / (EI / L) = %+.2e\n", zu, zr, zU);
	//draw
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Path Independence/plot.gp");
}
