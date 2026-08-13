//std
#include <cmath>

//Math
#include "Math/inc/Miscellaneous/util.hpp"

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

#include "FEA/inc/Geometry/Arc.hpp"
#include "FEA/inc/Geometry/Line.hpp"
#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t na = 5;
static const uint32_t nl = 5;
static const uint32_t nw = 1;
static const uint32_t nh = 40;
static const double a = 2.00e-04;
static const double P = 2.00e+00;
static const double t = 5.10e-04;
static const double v = 3.00e-01;
static const double E = 1.57e+09;
static const double Ep = 3.08e+06;
static const double ey = 2.85e-02;
static const double L1 = 4.00e-03;
static const double L2 = 1.20e-02;
static const double R0 = 4.00e-04;

static const double sy = E * ey;
static const double K = E * Ep / (E - Ep);

//reference: doi.org/10.48550/arXiv.2412.06022

static double time_function(double t)
{
	return t < 0.5 ? 2 * t : 2 * (1 - t);
}

void test::beam2D::inelastic::morpho_plastic_unit(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	//points
	model.geometry()->create_point(0, 0, 0);
	model.geometry()->create_point(R0, 0, 0);
	model.geometry()->create_point(R0, R0, 0);
	model.geometry()->create_point(L2 + R0, R0, 0);
	//half-right
	model.geometry()->create_line(2, 3);
	model.geometry()->create_arc(0, 1, 2);
	model.geometry()->curve(0)->move(0, 2 * R0, 0, true);
	model.geometry()->curve(1)->rotate(R0, R0, 0, 0, 0, 1, M_PI, true)->move(L2, 0, 0, false);
	model.geometry()->curve(1)->rotate(R0, R0, 0, 0, 0, 1, -M_PI_2, true)->move(L2 + R0, R0, 0, false);
	model.geometry()->curve(1)->rotate(R0, R0, 0, 0, 0, 1, +M_PI_2, true)->move(-R0, 3 * R0, 0, false);
	//half-left
	model.geometry()->scale_curves({0, 1, 2, 3, 4, 5}, 0, 0, 0, -1, 1, 1, true);
	model.geometry()->curve( 9)->move(L2 - L1, 0, 0, false);
	model.geometry()->curve(10)->move(L2 - L1, 0, 0, false);
	model.geometry()->curve( 6)->scale(-R0, 0, 0, L1 / L2, 1, 1, false);
	model.geometry()->curve( 8)->scale(-R0, 0, 0, L1 / L2, 1, 1, false);
	//curves
	for(fea::geometry::Curve* curve : model.geometry()->curves())
	{
		curve->element_type(fea::mesh::elements::Type::Beam2D);
		if(dynamic_cast<fea::geometry::Arc*>(curve)) curve->structured(na);
		if(dynamic_cast<fea::geometry::Line*>(curve)) curve->structured(nl);
	}
	materials::Mechanic::inelastic(true);
	//generate
	model.geometry()->merge();
	model.geometry()->generate_mesh();
	const uint32_t in = model.geometry()->curve(5)->index_point(2);
	//elements
	section.width(t);
	section.height(t);
	section.fibers_width(nw);
	section.fibers_height(nh);
	material.yield_stress(sy);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	material.plastic_modulus(K);
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam2D*) element)->section(&section);
		((fea::mesh::elements::Beam2D*) element)->material(&material);
	}
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(in, dof::Translation_1);
	//loads
	model.boundary()->create_load_combination(0, true);
	model.boundary()->create_load_case(in, dof::Translation_2, P);
	model.boundary()->load_case(0)->load_node(0)->time_function(time_function);
	//setup
	section.compute();
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->attempt_max(1);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(in);
	model.analysis()->solver_static_nonlinear()->convergence().tolerance(1.00e-05);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->convergence().type(math::solvers::Convergence::Type::Fixed);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::LoadControl);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Inelastic/Morpho Plastic Unit");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Inelastic/Morpho Plastic Unit/data.txt", {
		{0, dof::Rotation_3}, {in, dof::Translation_2}, {in, dof::Rotation_3}
	});
	//draw
	fea::draw::Engine(&model).start();
}