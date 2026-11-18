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
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Point.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t nd = 6;
static const uint32_t ne = 5;
static const double w = 7.600e-01;
static const double h = 1.220e+00;
static const double E = 2.069e+04;
static const double v = 1.716e-01;
static const double r1 = 1.257e+01;
static const double r2 = 2.438e+01;
static const double h1 = 1.550e+00;
static const double h2 = 4.550e+00;

//Reference: J-M. Battini, PhD Thesis (2002), pp. 146

void test::beam3D::elastic::dome_framed(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	//points
	for(uint32_t i = 0; i < nd; i++)
	{
		const double t = 2 * M_PI * i / nd;
		const double q = 2 * M_PI * i / nd + M_PI / 5000;
		model.geometry()->create_point(r2 * cos(t), r2 * sin(t), 0);
		model.geometry()->create_point(r1 * cos(q), r1 * sin(q), h2);
	}
	model.geometry()->create_point(0, 0, h1 + h2);
	//curves
	for(uint32_t i = 0; i < nd; i++)
	{
		model.geometry()->create_line(2 * i + 1, 2 * nd);
		model.geometry()->create_line(2 * i + 0, 2 * i + 1);
		model.geometry()->create_line(2 * i + 1, (2 * i + 3) % (2 * nd));
		for(uint32_t j = 0; j < 3; j++)
		{
			model.geometry()->curve(3 * i + j)->structured(ne);
			model.geometry()->curve(3 * i + j)->element_type(fea::mesh::elements::Type::Beam3D);
		}
	}
	//generate
	model.geometry()->generate_mesh();
	//elements
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam3D*) element)->section(&section);
		((fea::mesh::elements::Beam3D*) element)->material(&material);
	}
	for(uint32_t i = 0; i < nd; i++)
	{
		const double t = 2 * M_PI * i / nd + M_PI / 2;
		const double q = 2 * M_PI * i / nd + M_PI / nd;
		for(uint32_t j = 0; j < ne; j++)
		{
			((fea::mesh::elements::Beam3D*) model.mesh()->element(ne * (3 * i + 0) + j))->major_axis() = {cos(t), sin(t), 0};
			((fea::mesh::elements::Beam3D*) model.mesh()->element(ne * (3 * i + 1) + j))->major_axis() = {cos(t), sin(t), 0};
			((fea::mesh::elements::Beam3D*) model.mesh()->element(ne * (3 * i + 2) + j))->major_axis() = {cos(q), sin(q), 0};
		}
	}
	//supports
	for(uint32_t i = 0; i < nd; i++)
	{
		const uint32_t node = model.geometry()->point(2 * i)->node();
		model.boundary()->create_support(node, dof::Rotation_1);
		model.boundary()->create_support(node, dof::Rotation_2);
		model.boundary()->create_support(node, dof::Rotation_3);
		model.boundary()->create_support(node, dof::Translation_1);
		model.boundary()->create_support(node, dof::Translation_2);
		model.boundary()->create_support(node, dof::Translation_3);
	}
	//loads
	section.width(w);
	section.height(h);
	section.compute();
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	const uint32_t apex = model.geometry()->point(2 * nd)->node();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(apex, dof::Translation_3, -1);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(100);
	model.analysis()->solver_static_nonlinear()->step_adapt(true);
	model.analysis()->solver_static_nonlinear()->step_size(2.00e+00);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->iteration_desired(3);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(apex);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_3);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(2.00e+02);
	model.analysis()->solver_static_nonlinear()->continuation().type(math::solvers::Continuation::Type::StateControl);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Elastic/Dome Framed");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Elastic/Dome Framed/data.txt", {
		{apex, dof::Translation_1}, {apex, dof::Translation_2}, {apex, dof::Translation_3}
	});
	//validator
	validator.create_item();
	validator.item(0)->tolerance(3.50e-02);
	validator.item(0)->adjuster_numeric([](double& x1, double&){ x1 = -x1; });
	validator.item(0)->load_numeric("Test/data/Beam 3D/Elastic/Dome Framed/data.txt", 2, 3);
	validator.item(0)->load_reference("Test/data/Beam 3D/Elastic/Dome Framed/reference-2.dat", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
	fea::draw::Engine::plot("Test/data/Beam 3D/Elastic/Dome Framed/plot.gp");
}