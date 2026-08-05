//std
#include <cmath>

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
static const double b = 1.00e-01;
static const double h = 1.00e-01;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;

static const double A = b * h;
static const double I = b * h * h * h / 12;

//reference: doi.org/10.1002/nme.1620170113

void test::beam2D::elastic::diamond_frame(void)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	//points
	model.geometry()->create_point(-L * cos(M_PI_4), 0, 0);
	model.geometry()->create_point(0, -L * sin(M_PI_4), 0);
	model.geometry()->create_point(0, -L * sin(M_PI_4), 0);
	model.geometry()->create_point(+L * cos(M_PI_4), 0, 0);
	model.geometry()->create_point(0, +L * sin(M_PI_4), 0);
	model.geometry()->create_point(0, +L * sin(M_PI_4), 0);

	model.geometry()->create_point(-L * cos(M_PI_4), 0, 0);
	model.geometry()->create_point(0, -L * sin(M_PI_4), 0);
	model.geometry()->create_point(0, -L * sin(M_PI_4), 0);
	model.geometry()->create_point(+L * cos(M_PI_4), 0, 0);
	model.geometry()->create_point(0, +L * sin(M_PI_4), 0);
	model.geometry()->create_point(0, +L * sin(M_PI_4), 0);
	//curves
	model.geometry()->create_line( 0,  1);
	model.geometry()->create_line( 2,  3);
	model.geometry()->create_line( 3,  4);
	model.geometry()->create_line( 5,  0);

	model.geometry()->create_line( 6,  7);
	model.geometry()->create_line( 8,  9);
	model.geometry()->create_line( 9, 10);
	model.geometry()->create_line(11,  6);
	for(fea::geometry::Curve* curve : model.geometry()->curves())
	{
		curve->structured(ne);
		curve->element_type(fea::mesh::elements::Type::Beam2D);
	}
	//generate
	model.geometry()->generate_mesh();
	//elements
	section.area(A);
	section.inertia(1, I);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam2D*) element)->section(&section);
		((fea::mesh::elements::Beam2D*) element)->material(&material);
	}
	//supports
	model.boundary()->create_support(1, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(3, dof::Translation_2);

	model.boundary()->create_support(7, dof::Translation_1);
	model.boundary()->create_support(6, dof::Translation_2);
	model.boundary()->create_support(9, dof::Translation_2);
	//loads
	model.boundary()->create_load_case();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->load_case(0)->create_load_node( 1, dof::Translation_2, -2 * E * I / L / L);
	model.boundary()->load_case(0)->create_load_node( 4, dof::Translation_2, +2 * E * I / L / L);

	model.boundary()->load_case(0)->create_load_node( 7, dof::Translation_2, +2 * E * I / L / L);
	model.boundary()->load_case(0)->create_load_node(10, dof::Translation_2, -2 * E * I / L / L);
	//dependencies
	model.boundary()->create_dependency( 1, dof::Translation_1,  2, dof::Translation_1);
	model.boundary()->create_dependency( 1, dof::Translation_2,  2, dof::Translation_2);
	model.boundary()->create_dependency( 4, dof::Translation_1,  5, dof::Translation_1);
	model.boundary()->create_dependency( 4, dof::Translation_2,  5, dof::Translation_2);

	model.boundary()->create_dependency( 7, dof::Translation_1,  8, dof::Translation_1);
	model.boundary()->create_dependency( 7, dof::Translation_2,  8, dof::Translation_2);
	model.boundary()->create_dependency(10, dof::Translation_1, 11, dof::Translation_1);
	model.boundary()->create_dependency(10, dof::Translation_2, 11, dof::Translation_2);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(500);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(4);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(1.00e+01);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 2D/Diamond Frame");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Diamond Frame/data-tension.txt", {
		{0, dof::Translation_1}, { 4, dof::Translation_2}, { 5, dof::Rotation_3}
	});
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 2D/Diamond Frame/data-compression.txt", {
		{6, dof::Translation_1}, {10, dof::Translation_2}, {11, dof::Rotation_3}
	});
	//validator
	// validator.create_item();
	// validator.create_item();
	// validator.create_item();
	// validator.item(0)->load_numeric("Test/data/Beam 2D/Diamond Frame/data-tension.txt", 3, 0);
	// validator.item(1)->load_numeric("Test/data/Beam 2D/Diamond Frame/data-tension.txt", 3, 1);
	// validator.item(2)->load_numeric("Test/data/Beam 2D/Diamond Frame/data-tension.txt", 3, 2);
	// validator.item(0)->load_reference("Test/data/Beam 2D/Diamond Frame/reference-tension.dat", 0, 2);
	// validator.item(1)->load_reference("Test/data/Beam 2D/Diamond Frame/reference-tension.dat", 0, 1);
	// validator.item(2)->load_reference("Test/data/Beam 2D/Diamond Frame/reference-tension.dat", 0, 3);
	//validate
	// validator.validate();
	//draw
	fea::draw::Engine(&model).start();
}