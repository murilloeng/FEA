//Sections
#include "Sections/inc/Rectangle.hpp"

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

#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t nw = 1;
static const uint32_t nh = 40;
static const uint32_t ne = 10;
static const double w = 2.00e-01;
static const double h = 4.00e-01;
static const double L = 3.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double K = 0.00e+00;
static const double sy = 2.50e+08;

void test::beam2D::inelastic::cantilever_bending(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	//points
	model.geometry()->create_point(0, 0, 0);
	model.geometry()->create_point(L, 0, 0);
	//curves
	model.geometry()->create_line(0, 1);
	model.geometry()->curve(0)->structured(ne);
	model.geometry()->curve(0)->element_type(fea::mesh::elements::Type::Beam2D);
	//generate
	model.geometry()->generate_mesh();
	//elements
	section.width(w);
	section.height(h);
	section.fibers_width(nw);
	section.fibers_height(nh);
	material.yield_stress(sy);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	material.plastic_modulus(K);
	materials::Mechanic::inelastic(true);
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam2D*) element)->section(&section);
		((fea::mesh::elements::Beam2D*) element)->material(&material);
	}
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//supports
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//loads
	section.compute();
	const double W = section.plastic_modulus(1);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Rotation_3, sy * W);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Rotation_3);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Inelastic/Cantilever Bending");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Inelastic/Cantilever Bending/data.txt", {
		{1, dof::Translation_1}, {1, dof::Translation_2}, {1, dof::Rotation_3}
	});
	//draw
	fea::draw::Engine(&model).start();
}