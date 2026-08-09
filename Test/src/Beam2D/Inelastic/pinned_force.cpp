//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

#include "Math/inc/Quadrature/Rules.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Draw.hpp"
#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t ne = 5;
static const uint32_t nw = 1;
static const uint32_t nh = 40;
static const double w = 2.00e-01;
static const double h = 4.00e-01;
static const double L = 3.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double K = 0.00e+00;
static const double sy = 2.50e+08;

void test::beam2D::inelastic::pinned_force(void)
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
	model.geometry()->create_point(L / 2, 0, 0);
	//curves
	model.geometry()->create_line(0, 2);
	model.geometry()->create_line(1, 2);
	model.geometry()->curve(0)->structured(ne);
	model.geometry()->curve(1)->structured(ne);
	model.geometry()->curve(0)->element_type(fea::mesh::elements::Type::Beam2D);
	model.geometry()->curve(1)->element_type(fea::mesh::elements::Type::Beam2D);
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
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(1, dof::Translation_2);
	//loads
	section.compute();
	const double W = section.plastic_modulus(1);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(2, dof::Translation_2, -4 * sy * W / L);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(2);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::MinimalNorm);
	//solve
	model.solve();
	printf("Node 2: %+.2e\n", model.mesh()->node(2)->state(dof::Translation_2));
	//save
	model.save_results("Test/data/Beam 2D/Inelastic/Pinned Force");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Inelastic/Pinned Force/data.txt", {{2, dof::Translation_2}});
	//draw
	fea::draw::Engine engine(&model);
	engine.draw()->scale(10);
	engine.start();
}