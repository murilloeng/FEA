//std
#include <cmath>
#include <vector>

//Sections
#include "Sections/inc/Generic.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Linear/Matrix.hpp"
#include "Math/inc/Validation/Validator.hpp"

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

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t nt = 3;
static const uint32_t ne = 30;
static const double A = 1.00e+04;
static const double I = 1.00e+00;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 1.00e+00;
static const math::Vec3 d(1. / 3, 2. / 3, 2. / 3);
static const math::Vec3 m(2. / 3, -2. / 3, 1. / 3);

//Reference: exact pure bending of a cantilever (tip moment M = nt 2 pi EI / L, rolled up nt times) in a skew plane
//(axis d, moment axis m, both off the global axes): u = L (sin(t) / t - 1) d + L (1 - cos(t)) / t (m x d), t = M L / EI;
//the tip rotation passes +/- pi several times (recorded wrapped, unwrapped here)

static double function_ud(double p)
{
	const double t = 2 * M_PI * nt * p;
	return t == 0 ? 0 : L * (sin(t) / t - 1);
}
static double function_un(double p)
{
	const double t = 2 * M_PI * nt * p;
	return t == 0 ? 0 : L * (1 - cos(t)) / t;
}
static double function_tm(double p)
{
	return 2 * M_PI * nt * p;
}

void test::beam3D::elastic::roll_up(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//nodes
	for(uint32_t i = 0; i <= ne; i++)
	{
		const math::Vec3 z = L * i / ne * d;
		model.mesh()->create_node(z[0], z[1], z[2]);
	}
	//elements
	for(uint32_t i = 0; i < ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam3D, {i, i + 1});
		((fea::mesh::elements::Beam3D*) model.mesh()->element(i))->major_axis() = m;
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
	section.inertia(1, I);
	section.torsion_constant(I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	//loads
	const double M = 2 * M_PI * nt * E * I / L;
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->load_case(0)->create_load_node(ne, dof::Rotation_1, M * m[0]);
	model.boundary()->load_case(0)->create_load_node(ne, dof::Rotation_2, M * m[1]);
	model.boundary()->load_case(0)->create_load_node(ne, dof::Rotation_3, M * m[2]);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(400 * nt);
	model.analysis()->solver_static_nonlinear()->step_size(1.00 / 400 / nt);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_1);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Elastic/Roll Up");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Elastic/Roll Up/data.txt", {
		{ne, dof::Translation_1}, {ne, dof::Translation_2}, {ne, dof::Translation_3},
		{ne, dof::Rotation_1}, {ne, dof::Rotation_2}, {ne, dof::Rotation_3}
	});
	//projections (rotation recorded in [-pi, pi], unwrapped)
	const math::Vec3 n = m.cross(d);
	const fea::mesh::nodes::Node* node = model.mesh()->node(ne);
	const uint32_t ns = model.analysis()->solver_static_nonlinear()->step();
	std::vector<double> p(ns), ud(ns), un(ns), um(ns), tm(ns);
	for(uint32_t i = 0; i < ns; i++)
	{
		math::Vec3 u, r;
		for(uint32_t j = 0; j < 3; j++)
		{
			u[j] = model.analysis()->solver_static_nonlinear()->state_data(i, node->dof_index(dof(j + 0)));
			r[j] = model.analysis()->solver_static_nonlinear()->state_data(i, node->dof_index(dof(j + 3)));
		}
		ud[i] = u.inner(d);
		un[i] = u.inner(n);
		um[i] = u.inner(m);
		tm[i] = r.inner(m);
		p[i] = model.analysis()->solver_static_nonlinear()->load_data(i);
		if(i != 0) tm[i] += 2 * M_PI * round((tm[i - 1] - tm[i]) / 2 / M_PI);
	}
	//validator
	validator.create_item();
	validator.create_item();
	validator.create_item();
	validator.item(0)->function(function_ud);
	validator.item(1)->function(function_un);
	validator.item(2)->function(function_tm);
	validator.item(0)->load_numeric(p.data(), ud.data(), ns);
	validator.item(1)->load_numeric(p.data(), un.data(), ns);
	validator.item(2)->load_numeric(p.data(), tm.data(), ns);
	//validate
	double um_max = 0;
	for(double u : um) um_max = fmax(um_max, fabs(u) / L);
	if(um_max < 1.00e-08) validator.validate();
	else printf("Status: ❌ - out of plane motion: max |u m| / L = %+.2e\n", um_max);
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Roll Up/plot.gp");
}
