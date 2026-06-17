//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Elements/Types.hpp"

int main(void)
{
	try
	{
		fea::Model model;
		model.mesh()->create_node(0, 0, 0);
		model.mesh()->create_node(1, 1, 0);
		model.mesh()->create_node(2, 0, 0);
		model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {0, 1});
		model.mesh()->create_element(fea::mesh::elements::Type::Truss2D, {1, 2});
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}