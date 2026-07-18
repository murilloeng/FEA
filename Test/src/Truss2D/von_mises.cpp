//std
#include <cmath>

//Test
#include "FEA/Test/inc/Truss2D.hpp"

//Sections
#include "Sections/inc/CHS.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Validation/Validator.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//data
static const double H = 1.00e-01;
static const double R = 1.00e+00;
static const double E = 2.10e+11;
static const double L = sqrt(H * H + R * R);

static double function(double u)
{
	return -(1 + u / H) * (2 + u / H) * u / H;
}

void test::truss2D::von_mises(void)
{
	//data
	fea::Model model;
	sections::CHS section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//nodes
	model.mesh()->create_node(-R, 0, 0);
	model.mesh()->create_node(+0, H, 0);
	model.mesh()->create_node(+R, 0, 0);
	//elements
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {0, 1});
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {1, 2});
	((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->section(&section);
	((fea::mesh::elements::Truss2D*) model.mesh()->element(1))->section(&section);
	((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->material(&material);
	((fea::mesh::elements::Truss2D*) model.mesh()->element(1))->material(&material);
	fea::mesh::elements::Truss::strain_measure(fea::mesh::elements::StrainMeasure::Quadratic);
	//supports
	model.boundary()->create_support(0, fea::mesh::nodes::DOF::Translation_1);
	model.boundary()->create_support(0, fea::mesh::nodes::DOF::Translation_2);
	model.boundary()->create_support(2, fea::mesh::nodes::DOF::Translation_1);
	model.boundary()->create_support(2, fea::mesh::nodes::DOF::Translation_2);
	//loads
	section.compute();
	material.elastic_modulus(E);
	const double A = section.area();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, fea::mesh::nodes::DOF::Translation_2, -E * A * pow(H / L, 3));
	//setup
	model.analysis()->type(fea::analysis::Type::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->stability(true);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(fea::mesh::nodes::DOF::Translation_2);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Truss 2D/von Mises");
	model.analysis()->solver()->save("Test/data/Truss 2D/von Mises/data.txt");
	//validator
	validator.create_item();
	validator.item(0)->function(function);
	validator.item(0)->load_numeric("Test/data/Truss 2D/von Mises/data.txt", 1, 2);
	//validate
	validator.validate();
}