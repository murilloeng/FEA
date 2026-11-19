//std
#include <cmath>
#include <cstdio>
#include <cstring>

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Groups/ASO3.hpp"
#include "Math/inc/Groups/GSO3.hpp"
#include "Math/inc/Linear/Matrix.hpp"
#include "Math/inc/Miscellaneous/util.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Joints/Type.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Rigid3D.hpp"

//data
static const uint32_t nt = 1000;
static const uint32_t ne = 10;
static const double s = 1.00e-06;
static const double b = 1.00e-01;
static const double h = 1.00e-01;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;

void test::rigid3D::derivatives(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::mesh::joints::Type joint;
	typedef fea::mesh::elements::Type element;
	//nodes
	for(uint32_t i = 0; i <= ne; i++)
	{
		model.mesh()->create_node(L * i / ne, 0, 0);
	}
	model.mesh()->create_node(L + math::randu(), math::randu(), math::randu());
	//joints
	model.mesh()->create_joint(joint::Rigid3D, {ne, ne + 1});
	//elements
	for(uint32_t i = 0; i < ne; i++)
	{
		model.mesh()->create_element(element::Beam3D, {i + 0, i + 1});
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
	section.width(b);
	section.height(h);
	section.compute();
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	const double I = section.inertia(0);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(ne + 1, dof::Rotation_1, E * I / L);
	model.boundary()->create_load_case(ne + 1, dof::Rotation_2, E * I / L);
	model.boundary()->create_load_case(ne + 1, dof::Rotation_3, E * I / L);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(100);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne + 1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Rotation_3);
	//solve (committed rotations away from identity)
	model.solve();
	model.check();
	printf("Committed rotation angle: %+.6e\n", model.mesh()->node(ne)->rotation_gso3().log().vector().norm());
	//test
	double eg = 0, eH = 0;
	for(const fea::boundary::Constraint* constraint : model.boundary()->constraints())
	{
		//data
		const uint32_t nd = constraint->dof().size();
		double x[5], xp[5], xm[5], f, fp, fm, g[5], gp[5], gm[5], H[25];
		for(uint32_t k = 0; k < nt; k++)
		{
			//state
			for(uint32_t i = 0; i < nd; i++) x[i] = math::randu(-M_PI, M_PI);
			constraint->gradient()(g, x);
			constraint->hessian()(H, x);
			constraint->function()(f, x);
			//gradient: df/dx = g * T on rotations
			double gn[5], ga[5];
			for(uint32_t i = 0; i < nd; i++)
			{
				memcpy(xp, x, nd * sizeof(double));
				memcpy(xm, x, nd * sizeof(double));
				xp[i] += s;
				xm[i] -= s;
				constraint->function()(fp, xp);
				constraint->function()(fm, xm);
				gn[i] = (fp - fm) / 2 / s;
			}
			memcpy(ga, g, nd * sizeof(double));
			if(nd == 5)
			{
				const math::Mat3 T = math::ASO3(-x[2], -x[3], -x[4]).jacobian();
				const math::Vec3 gt = T.transpose() * math::Vec3(g + 2);
				for(uint32_t i = 0; i < 3; i++) ga[2 + i] = gt[i];
			}
			for(uint32_t i = 0; i < nd; i++) eg = fmax(eg, fabs(ga[i] - gn[i]));
			//hessian: dg/dx
			for(uint32_t j = 0; j < nd; j++)
			{
				memcpy(xp, x, nd * sizeof(double));
				memcpy(xm, x, nd * sizeof(double));
				xp[j] += s;
				xm[j] -= s;
				constraint->gradient()(gp, xp);
				constraint->gradient()(gm, xm);
				for(uint32_t i = 0; i < nd; i++) eH = fmax(eH, fabs(H[i + nd * j] - (gp[i] - gm[i]) / 2 / s));
			}
		}
	}
	//print
	printf("Constraints: %zd\n", model.boundary()->constraints().size());
	printf("Rigid 3D derivatives: gradient error: %+.2e hessian error: %+.2e\n", eg, eH);
	printf("Status: %s\n", eg < 1e-6 && eH < 1e-6 ? "✅" : "❌");
}