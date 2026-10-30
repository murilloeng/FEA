//std
#include <cmath>
#include <string>

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

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t ne = 4;
static const double b = 5.00e-01;
static const double h = 5.00e-01;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;

static const double A = b * h;
static const double As = 5 * A / 6;
static const double I = b * h * h * h / 12;
static const double G = E / 2 / (1 + v);
static const double P = 3.00e-06 * E * I / L / L / L;

//Reference: linear Timoshenko beam theory, tip deflection P L^3 / (3 E I) + P L / (G As)

static void solve(bool shear, const char* name)
{
	//data
	fea::Model model;
	sections::Generic section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	const std::string path = std::string("Test/data/Beam 2D/Elastic/Timoshenko Cantilever/data-") + name + ".txt";
	const double d = P * L * L * L / 3 / E / I + (shear ? P * L / G / As : 0);
	//types
	typedef fea::mesh::nodes::DOF dof;
	//nodes
	for(uint32_t i = 0; i <= ne; i++)
	{
		model.mesh()->create_node(L * i / ne, 0, 0);
	}
	//elements
	section.area(A);
	section.inertia(1, I);
	section.shear_area(0, As);
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	for(uint32_t i = 0; i < ne; i++)
	{
		model.mesh()->create_element(fea::mesh::elements::Type::Beam2D, {i, i + 1});
		((fea::mesh::elements::Beam2D*) model.mesh()->element(i))->section(&section);
		((fea::mesh::elements::Beam2D*) model.mesh()->element(i))->material(&material);
	}
	fea::mesh::elements::Beam::shear(shear);
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//supports
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//loads
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(ne, dof::Translation_2, -P);
	//setup
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->silent(true);
	model.analysis()->solver_static_nonlinear()->step_max(100);
	model.analysis()->solver_static_nonlinear()->step_size(1.00e-01);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(ne);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_2);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(1.00e+00);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	fea::mesh::elements::Beam::shear(false);
	//save
	model.analysis()->solver_static_nonlinear()->save(path.c_str(), {{ne, dof::Translation_2}});
	//validator
	validator.create_item();
	validator.item(0)->function([d](double p){ return p * d; });
	validator.item(0)->adjuster_numeric([](double&, double& x2){ x2 = -x2; });
	validator.item(0)->load_numeric(path.c_str(), 1, 0);
	//validate
	printf("Timoshenko Cantilever (shear %s):\n", shear ? "on" : "off");
	validator.validate();
}

void test::beam2D::elastic::timoshenko_cantilever(void)
{
	//solve
	solve(false, "bending");
	solve(true, "shear");
	//draw
	fea::draw::Engine::plot("Test/data/Beam 2D/Elastic/Timoshenko Cantilever/plot.gp");
}
