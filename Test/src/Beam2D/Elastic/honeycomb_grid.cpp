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
static const uint32_t n1 = 11;
static const uint32_t n2 = 11;
static const double a = 1.00e-01;
static const double t = 1.00e-03;
static const double v = 3.00e-01;
static const double E = 2.10e+11;
static const double P = 1.00e+01;

//reference: doi.org/10.1002/nme.6820

void test::beam2D::elastic::honeycomb_grid(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	//points
	double x2 = 0;
	srand(time(nullptr));
	for(uint32_t i = 0; i < 2 * (n2 + 1); i++)
	{
		double x1 = (i % 4 == 0 || i % 4 == 3) * a * cos(M_PI / 6);
		for(uint32_t j = 0; j < n1 + 1; j++)
		{
			const double a1 = 1e-5 * a * math::randu(-1, 1);
			const double a2 = 1e-5 * a * math::randu(-1, 1);
			if((i % 4 == 0 || i % 4 == 3) && j == n1) continue;
			model.geometry()->create_point(x1 + a1, x2 + a2, 0);
			x1 += 2 * a * cos(M_PI / 6);
		}
		x2 += i % 2 == 0 ? a * sin(M_PI / 6) : a;
	}
	//curves
	for(uint32_t i = 0; i + 1 < 2 * (n2 + 1); i++)
	{
		//data
		const uint32_t k = (4 * n1 + 2) * (i / 4);
		//curves
		if(i % 4 == 0)
		{
			for(uint32_t j = 0; j < n1; j++)
			{
				model.geometry()->create_line(k + j, k + n1 + j + 0);
				model.geometry()->create_line(k + j, k + n1 + j + 1);
			}
		}
		if(i % 4 == 1)
		{
			for(uint32_t j = 0; j <= n1; j++)
			{
				model.geometry()->create_line(k + n1 + j, k + 2 * n1 + 1 + j);
			}
		}
		if(i % 4 == 2)
		{
			for(uint32_t j = 0; j < n1; j++)
			{
				model.geometry()->create_line(k + 3 * n1 + 2 + j, k + 2 * n1 + 1 + j + 0);
				model.geometry()->create_line(k + 3 * n1 + 2 + j, k + 2 * n1 + 1 + j + 1);
			}
		}
		if(i % 4 == 3)
		{
			for(uint32_t j = 0; j < n1; j++)
			{
				model.geometry()->create_line(k + 3 * n1 + 2 + j, k + 4 * n1 + 2 + j);
			}
		}
	}
	for(fea::geometry::Curve* curve : model.geometry()->curves())
	{
		curve->structured(ne);
		curve->element_type(fea::mesh::elements::Type::Beam2D);
	}
	//generate
	model.geometry()->generate_mesh();
	//elements
	section.width(t);
	section.height(t);
	section.compute();
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam2D*) element)->section(&section);
		((fea::mesh::elements::Beam2D*) element)->material(&material);
	}
	//supports
	for(uint32_t i = 0; i < n1; i++)
	{
		model.boundary()->create_support(i, dof::Translation_2);
	}
	model.boundary()->create_support(0, dof::Translation_1);
	//loads
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	for(uint32_t i = 0; i < n1; i++)
	{
		model.boundary()->load_case(0)->create_load_node((2 * n1 + 1) * (n2 + 1) - n1 + i, dof::Translation_2, P);
	}
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->iteration_max(30);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->convergence().tolerance(1.00e-05);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->watch_dof().node((2 * n1 + 1) * (n2 + 1) - n1);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::MinimalNorm);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//draw
	fea::draw::Engine(&model).start();
}