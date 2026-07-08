//Test
#include "FEA/Test/inc/Truss2D.hpp"

//Sections
#include "Sections/inc/CHS.hpp"

//Materials
#include "Materials/inc/Mechanic/Steel.hpp"

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
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

void test::truss2D::single(void)
{
	//data
	fea::Model model;
	sections::CHS section;
	materials::Steel material;
	const double L = 1.00e+00;
	//nodes
	model.mesh()->create_node(0, 0, 0);
	model.mesh()->create_node(L, 0, 0);
	//sections
	section.compute();
	//elements
	model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {0, 1});
	((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->section(&section);
	((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->material(&material);
	//supports
	model.boundary()->create_support(0, fea::mesh::nodes::DOF::Translation_1);
	model.boundary()->create_support(0, fea::mesh::nodes::DOF::Translation_2);
	model.boundary()->create_support(1, fea::mesh::nodes::DOF::Translation_2);
	//loads
	const double A = section.area();
	const double E = material.elastic_modulus();
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, fea::mesh::nodes::DOF::Translation_1, 0.01 * E * A / L);
	//solver
	model.analysis()->solver()->load_combination(0);
	//solve
	model.solve();
	printf("displacement: %+.2e\n", model.mesh()->node(1)->state(fea::mesh::nodes::DOF::Translation_1));
}