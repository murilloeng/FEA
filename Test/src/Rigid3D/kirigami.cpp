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
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Joints/Type.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Rigid3D.hpp"

//data
static const uint32_t na = 10;
static const uint32_t ns = 8;
static const uint32_t ne = 4;
static const double R = 1.00e+00;
static const double b = 1.00e-03;
static const double h = 1.00e-02;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double P = 1.00e+02;

//geometry: annulus k in [1, na) spans the radii [r(k), r(k + 1)] and its sections are staggered by half a section
static double radius(double k)
{
	return R * k / na;
}
static double angle(uint32_t k, uint32_t j, double s)
{
	return 2 * M_PI / ns * (j + s + 0.5 * k);
}

void test::rigid3D::kirigami(void)
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
	uint32_t nn = 0;
	uint32_t hubs[na][ns][2][2];
	const std::function<uint32_t(double, double)> node = [&model, &nn](double r, double t){
		model.mesh()->create_node(r * cos(t), r * sin(t), 0);
		return nn++;
	};
	//hubs (middle third): central node rigidly joined to a fork at each end (inner and outer radius)
	for(uint32_t k = 1; k < na; k++)
	{
		for(uint32_t j = 0; j < ns; j++)
		{
			const uint32_t nc = node(radius(k + 0.5), angle(k, j, 1.0 / 2));
			for(uint32_t a = 0; a < 2; a++)
			{
				for(uint32_t b = 0; b < 2; b++)
				{
					hubs[k][j][a][b] = node(radius(k + b), angle(k, j, (1.0 + a) / 3));
					model.mesh()->create_joint(joint::Rigid3D, {nc, hubs[k][j][a][b]});
				}
			}
		}
	}
	//ligaments (end thirds): beams along the circle r from the fork node n1 (angle t1) to the fork node n2 (angle t2)
	const std::function<void(uint32_t, uint32_t, double, double, double)> ligament = [&model, &node](uint32_t n1, uint32_t n2, double r, double t1, double t2){
		for(uint32_t i = 1; i <= ne; i++)
		{
			const uint32_t n = i == ne ? n2 : node(r, t1 + (t2 - t1) * i / ne);
			model.mesh()->create_element(element::Beam3D, {n1, n});
			n1 = n;
		}
	};
	//ligaments: annulus k connects with annulus k + 1 along the circle r(k + 1)
	for(uint32_t k = 1; k + 1 < na; k++)
	{
		for(uint32_t j = 0; j < ns; j++)
		{
			ligament(hubs[k][j][1][1], hubs[k + 1][j][0][0], radius(k + 1), angle(k, j, 2.0 / 3), angle(k + 1, j, 1.0 / 3));
			ligament(hubs[k + 1][j][1][0], hubs[k][(j + 1) % ns][0][1], radius(k + 1), angle(k + 1, j, 2.0 / 3), angle(k, j + 1, 1.0 / 3));
		}
	}
	//ligaments: the innermost and outermost annulus connect their neighbor sections along the circles r(1) and r(na)
	for(uint32_t j = 0; j < ns; j++)
	{
		ligament(hubs[1][j][1][0], hubs[1][(j + 1) % ns][0][0], radius(1), angle(1, j, 2.0 / 3), angle(1, j + 1, 1.0 / 3));
		ligament(hubs[na - 1][j][1][1], hubs[na - 1][(j + 1) % ns][0][1], radius(na), angle(na - 1, j, 2.0 / 3), angle(na - 1, j + 1, 1.0 / 3));
	}
	//elements
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam3D*) element)->section(&section);
		((fea::mesh::elements::Beam3D*) element)->material(&material);
	}
	//section
	section.width(b);
	section.height(h);
	section.compute();
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	//boundary: nodes on the inner (r(1)) and outer (r(na)) circles
	std::vector<uint32_t> nodes_inner, nodes_outer;
	for(const fea::mesh::nodes::Node* node : model.mesh()->nodes())
	{
		const double r = hypot(node->position()[0], node->position()[1]);
		if(fabs(r - radius(1)) < 1e-6 * R) nodes_inner.push_back(node->index());
		if(fabs(r - radius(na)) < 1e-6 * R) nodes_outer.push_back(node->index());
	}
	//supports: outer circle vertically supported and one outer beam node fully supported
	for(uint32_t node : nodes_outer)
	{
		bool tip = false;
		for(uint32_t j = 0; j < ns; j++) tip = tip || node == hubs[na - 1][j][0][1] || node == hubs[na - 1][j][1][1];
		if(tip) continue;
		model.boundary()->create_support(node, dof::Translation_1);
		model.boundary()->create_support(node, dof::Translation_2);
		model.boundary()->create_support(node, dof::Translation_3);
	}
	for(uint32_t j = 0; j < ns; j++)
	{
		const uint32_t center = hubs[na - 1][j][0][0] - 1;
		model.boundary()->create_support(center, dof::Translation_1);
		model.boundary()->create_support(center, dof::Translation_2);
		model.boundary()->create_support(center, dof::Translation_3);
		model.boundary()->create_support(center, dof::Rotation_1);
		model.boundary()->create_support(center, dof::Rotation_2);
		model.boundary()->create_support(center, dof::Rotation_3);
	}
	//loads: inner circle vertical force P distributed over its nodes
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	for(uint32_t node : nodes_inner)
	{
		model.boundary()->load_case(0)->create_load_node(node, dof::Translation_3, P / nodes_inner.size());
	}
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(500);
	model.analysis()->solver_static_nonlinear()->step_adapt(true);
	model.analysis()->solver_static_nonlinear()->step_size(1.00e-01);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->iteration_desired(4);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(nodes_inner[0]);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_3);
	model.analysis()->solver_static_nonlinear()->convergence().type_add(math::solvers::Convergence::Type::State);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::ArcLengthCylindrical);
	model.analysis()->solver_static_nonlinear()->stop_criteria().state_max(8.00e-01 * R);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::StateLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Rigid 3D/Kirigami");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Rigid 3D/Kirigami/data.txt", {
		{nodes_inner[0], dof::Translation_3}
	});
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Rigid 3D/Kirigami/plot.gp");
}