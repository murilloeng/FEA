//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Steel.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Elements/Types.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

int main(void)
{
	try
	{
		//data
		fea::Model model;
		materials::Steel material;
		sections::Rectangle section;
		//section
		section.width(1.00e-02);
		section.height(1.00e-02);
		//nodes
		model.mesh()->create_node(0, 0, 0);
		model.mesh()->create_node(1, 1, 0);
		model.mesh()->create_node(2, 0, 0);
		//elements
		model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {0, 1});
		model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {1, 2});
		((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->section(&section);
		((fea::mesh::elements::Truss2D*) model.mesh()->element(1))->section(&section);
		((fea::mesh::elements::Truss2D*) model.mesh()->element(0))->material(&material);
		((fea::mesh::elements::Truss2D*) model.mesh()->element(1))->material(&material);
		//supports
		model.boundary()->create_support(0, fea::mesh::nodes::dof::translation_1);
		model.boundary()->create_support(0, fea::mesh::nodes::dof::translation_2);
		model.boundary()->create_support(2, fea::mesh::nodes::dof::translation_1);
		model.boundary()->create_support(2, fea::mesh::nodes::dof::translation_2);
		//loads
		model.boundary()->create_load_combination();
		model.boundary()->create_load_case(1, fea::mesh::nodes::dof::translation_2, -1e7);
		//solver
		model.analysis()->solver(fea::analysis::solvers::Type::StaticNonlinear);
		dynamic_cast<fea::analysis::solvers::StaticNonlinear*>(model.analysis()->solver())->load_combination(0);
		dynamic_cast<fea::analysis::solvers::StaticNonlinear*>(model.analysis()->solver())->watch_dof(1, fea::mesh::nodes::dof::translation_2);
		//solve
		section.compute();
		model.analysis()->solve();
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}