//std
#include <cmath>

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
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

void test::truss2D::von_mises(void)
{
	//data
	fea::Model model;
	sections::CHS section;
	materials::Steel material;
	const double H = 1.00e-01;
	const double R = 1.00e+00;
	const double L = sqrt(H * H + R * R);
	//nodes
	model.m_mesh->create_node(-R, 0, 0);
	model.m_mesh->create_node(+0, H, 0);
	model.m_mesh->create_node(+R, 0, 0);
	//elements
	model.m_mesh->create_element(fea::mesh::elements::Type::Truss2D, {0, 1});
	model.m_mesh->create_element(fea::mesh::elements::Type::Truss2D, {1, 2});
	((fea::mesh::elements::Truss2D*) model.m_mesh->m_elements[0])->m_section = &section;
	((fea::mesh::elements::Truss2D*) model.m_mesh->m_elements[1])->m_section = &section;
	((fea::mesh::elements::Truss2D*) model.m_mesh->m_elements[0])->m_material = &material;
	((fea::mesh::elements::Truss2D*) model.m_mesh->m_elements[1])->m_material = &material;
	//supports
	model.m_boundary->create_support(0, fea::mesh::nodes::DOF::Translation_1);
	model.m_boundary->create_support(0, fea::mesh::nodes::DOF::Translation_2);
	model.m_boundary->create_support(2, fea::mesh::nodes::DOF::Translation_1);
	model.m_boundary->create_support(2, fea::mesh::nodes::DOF::Translation_2);
	//loads
	section.compute();
	const double A = section.area();
	const double E = material.elastic_modulus();
	model.m_boundary->create_load_combination(0, false, 1);
	model.m_boundary->create_load_case(1, fea::mesh::nodes::DOF::Translation_2, -E * A * pow(H / L, 3));
	//solver
	model.m_analysis->create_solver(fea::analysis::Type::StaticNonlinear);
	//setup
	model.m_analysis->m_solver->m_load_combination = 0;
	model.m_analysis->m_solver->m_watch_dof.m_node = 1;
	model.m_analysis->m_solver->m_watch_dof.m_dof = fea::mesh::nodes::DOF::Translation_2;
	dynamic_cast<fea::analysis::StaticNonlinear*>(model.m_analysis->m_solver)->m_step_max = 400;
	//save
	model.m_analysis->m_solver->save("minha rola.txt");
	//solve
	model.solve();
}