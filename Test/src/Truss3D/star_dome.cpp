//std
#include <cmath>
#include <cstdio>

//Test
#include "FEA/Test/inc/Truss3D.hpp"

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
#include "FEA/inc/Mesh/Elements/Mechanic/Truss3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

using dof = fea::mesh::nodes::DOF;
using strain = fea::mesh::elements::StrainMeasure;
using continuation = math::solvers::Continuation::Type;

/*
	24 bar shallow star dome under a crown load, Quadratic measure. Usual shape: outer ring radius 50 at height 0, inner
	ring radius 25 at height 6.216, crown at height 8.216. The stiffness EA is not a literature match.
	Reference: an independent solve of the symmetric equilibrium equations, not published data. For a crown displacement
	u, the ring radius r and height z come from Newton on the ring equations, and the crown load from equilibrium.
	The analysis stops at a load factor of lmax, well before the crown snap-back around u = -13.
*/

//data
static const double R1 = 2.50e+01;
static const double R2 = 5.00e+01;
static const double H1 = 8.216e+00;
static const double H2 = 6.216e+00;
static const double E = 1.00e+06;
static const double A = 1.00e+00;
static const double P = 3.00e+02;
static const double lmax = 3.00e+00;

static double force(double l, double l0)
{
	const double s = l / l0;
	return E * A * s * (s * s - 1) / 2;
}
static void residue(double* f, double zc, double r, double z)
{
	//lengths
	const double c = cos(M_PI / 6);
	const double l1 = sqrt(r * r + (zc - z) * (zc - z));
	const double l2 = sqrt(r * r + R2 * R2 - 2 * r * R2 * c + z * z);
	const double l10 = sqrt(R1 * R1 + (H1 - H2) * (H1 - H2));
	const double l20 = sqrt(R1 * R1 + R2 * R2 - 2 * R1 * R2 * c + H2 * H2);
	//forces
	const double n1 = force(l1, l10);
	const double n2 = force(l2, l20);
	const double n3 = force(r, R1);
	//equilibrium
	f[0] = 6 * n1 * (zc - z) / l1;
	f[1] = 6 * n1 * r / l1 + 6 * n3 + 12 * n2 * (r - R2 * c) / l2;
	f[2] = -6 * n1 * (zc - z) / l1 + 12 * n2 * z / l2;
}
static double function(double u)
{
	//continuation from the undeformed shape
	const double h = 1.00e-6;
	double r = R1, z = H2, f[3], fp[3], fm[3];
	const uint32_t ns = uint32_t(fabs(u) / 2.50e-1) + 1;
	for(uint32_t i = 1; i <= ns; i++)
	{
		const double zc = H1 + u * i / ns;
		for(uint32_t k = 0; k < 20; k++)
		{
			double J[4];
			residue(f, zc, r, z);
			residue(fp, zc, r + h, z);
			residue(fm, zc, r - h, z);
			J[0] = (fp[1] - fm[1]) / (2 * h);
			J[2] = (fp[2] - fm[2]) / (2 * h);
			residue(fp, zc, r, z + h);
			residue(fm, zc, r, z - h);
			J[1] = (fp[1] - fm[1]) / (2 * h);
			J[3] = (fp[2] - fm[2]) / (2 * h);
			const double d = J[0] * J[3] - J[1] * J[2];
			r -= (J[3] * f[1] - J[1] * f[2]) / d;
			z -= (J[0] * f[2] - J[2] * f[1]) / d;
		}
	}
	residue(f, H1 + u, r, z);
	return -f[0] / P;
}

void test::truss3D::star_dome(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//nodes
	model.mesh()->create_node(0, 0, H1);
	for(uint32_t i = 0; i < 6; i++)
	{
		const double t = M_PI / 3 * i;
		model.mesh()->create_node(R1 * cos(t), R1 * sin(t), H2);
	}
	for(uint32_t i = 0; i < 6; i++)
	{
		const double t = M_PI / 3 * i + M_PI / 6;
		model.mesh()->create_node(R2 * cos(t), R2 * sin(t), 0);
	}
	//elements
	section.area(A);
	material.elastic_modulus(E);
	for(uint32_t i = 0; i < 6; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Truss3D, {0, i + 1});
		model.mesh()->create_element(fea::mesh::elements::Type::Truss3D, {i + 1, (i + 1) % 6 + 1});
		model.mesh()->create_element(fea::mesh::elements::Type::Truss3D, {i + 1, i + 7});
		model.mesh()->create_element(fea::mesh::elements::Type::Truss3D, {i + 1, (i + 5) % 6 + 7});
	}
	for(uint32_t i = 0; i < 24; i++)
	{
		((fea::mesh::elements::Truss3D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Truss3D*) model.mesh()->element(i))->material(&material);
	}
	fea::mesh::elements::Truss::strain_measure(strain::Quadratic);
	//supports
	for(uint32_t i = 0; i < 6; i++)
	{
		model.boundary()->create_support(i + 7, dof::Translation_1);
		model.boundary()->create_support(i + 7, dof::Translation_2);
		model.boundary()->create_support(i + 7, dof::Translation_3);
	}
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(0, dof::Translation_3, -P);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->step_size(5.00e-02);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(0);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(lmax);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_3);
	model.analysis()->solver_static_nonlinear()->continuation().type(continuation::ArcLengthCylindrical);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Truss 3D/Star Dome");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Truss 3D/Star Dome/data.txt", {{0, dof::Translation_3}});
	//validator
	validator.create_item();
	validator.item(0)->tolerance(1.00e-4);
	validator.item(0)->function(function);
	validator.item(0)->load_numeric("Test/data/Truss 3D/Star Dome/data.txt", 0, 1);
	//validate
	validator.validate();
	//reference
	FILE* file = fopen("Test/data/Truss 3D/Star Dome/reference.txt", "w");
	for(uint32_t i = 0; i <= 120; i++)
	{
		const double u = -6.00e+00 * i / 120;
		fprintf(file, "%+.6e %+.6e\n", u, function(u));
	}
	fclose(file);
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Truss 3D/Star Dome/plot.gp");
}
