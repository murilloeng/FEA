//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

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

//Sections
#include "Sections/inc/CHS.hpp"

//Materials
#include "Materials/inc/Mechanic/Steel.hpp"

int main(void)
{
	try
	{
		//data
		fea::Model model;
		sections::CHS section;
		materials::Steel material;
		const double L = 1.00e+00;
		//nodes
		model.m_mesh->create_node(0, 0, 0);
		model.m_mesh->create_node(L, 0, 0);
		//sections
		section.compute();
		//elements
		model.m_mesh->create_element(fea::mesh::elements::Type::Truss2D, {0, 1});
		((fea::mesh::elements::Truss2D*) model.m_mesh->m_elements[0])->m_section = &section;
		((fea::mesh::elements::Truss2D*) model.m_mesh->m_elements[0])->m_material = &material;
		//supports
		model.m_boundary->create_support(0, fea::mesh::nodes::DOF::Translation_1);
		model.m_boundary->create_support(0, fea::mesh::nodes::DOF::Translation_2);
		model.m_boundary->create_support(1, fea::mesh::nodes::DOF::Translation_2);
		//loads
		const double A = section.area();
		const double E = material.elastic_modulus();
		model.m_boundary->create_load_combination(0, false, 1);
		model.m_boundary->create_load_case(1, fea::mesh::nodes::DOF::Translation_1, 0.01 * E * A / L);
		//solver
		model.m_analysis->m_solver->m_load_combination = 0;
		//solve
		model.solve();
		printf("displacement: %+.2e\n", model.m_mesh->m_nodes[1]->state(fea::mesh::nodes::DOF::Translation_1));
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}