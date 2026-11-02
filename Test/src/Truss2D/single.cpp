//std
#include <cmath>
#include <string>

//Test
#include "FEA/Test/inc/Truss2D.hpp"

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
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

using dof = fea::mesh::nodes::DOF;
using strain = fea::mesh::elements::StrainMeasure;
using continuation = math::solvers::Continuation::Type;

/*
	Single bar along x, node 1 free to move axially only.
	Exact axial force from the Truss element work conjugate, s = stretch:
		Linear:      N = EA * (s - 1)
		Quadratic:   N = EA * s * (s * s - 1) / 2
		Logarithmic: N = EA * log(s) / s
	The path uses state control, which also crosses the Quadratic limit point at s = 1 / sqrt(3) in compression.
	The step size is the first load increment, so it is the state increment times the initial stiffness EA / L.
*/

//data
static const double L = 1.00e+00;
static const double A = 1.00e+00;
static const double E = 1.00e+00;
static const uint32_t ns = 100;

static double force(strain measure, double s)
{
	return E * A * (
		measure == strain::Linear ? s - 1 :
		measure == strain::Quadratic ? s * (s * s - 1) / 2 : log(s) / s
	);
}

static void run(strain measure, double stretch, const char* name)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	const std::string path = std::string("Test/data/Truss 2D/Single/") + name + ".txt";
	//nodes
	model.mesh()->create_node(0, 0, 0);
	model.mesh()->create_node(L, 0, 0);
	//elements
	section.area(A);
	material.elastic_modulus(E);
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {0, 1});
	((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->section(&section);
	((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->material(&material);
	fea::mesh::elements::Truss::strain_measure(measure);
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(1, dof::Translation_2);
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Translation_1, 1);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(ns);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_1);
	model.analysis()->solver_static_nonlinear()->step_size(E * A / L * (stretch - 1) * L / ns);
	model.analysis()->solver_static_nonlinear()->continuation().type(continuation::StateControl);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Truss 2D/Single");
	model.analysis()->solver_static_nonlinear()->save(path.c_str(), {{1, dof::Translation_1}});
	//validator
	validator.create_item();
	validator.item(0)->tolerance(1.00e-4);
	validator.item(0)->function([measure](double u){ return force(measure, 1 + u / L); });
	validator.item(0)->load_numeric(path.c_str(), 0, 1);
	//validate
	printf("%s: ", name);
	validator.validate();
}

void test::truss2D::single(void)
{
	run(strain::Linear, 2.00e+00, "Linear Tension");
	run(strain::Linear, 5.00e-01, "Linear Compression");
	run(strain::Quadratic, 2.00e+00, "Quadratic Tension");
	run(strain::Quadratic, 5.00e-01, "Quadratic Compression");
	run(strain::Logarithmic, 2.00e+00, "Logarithmic Tension");
	run(strain::Logarithmic, 5.00e-01, "Logarithmic Compression");
	fea::draw::Engine::plot("Test/data/Truss 2D/Single/plot.gp");
}
