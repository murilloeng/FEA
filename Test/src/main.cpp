//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//Test
#include "FEA/Test/inc/Beam2D.hpp"
#include "FEA/Test/inc/Beam3D.hpp"
#include "FEA/Test/inc/Truss2D.hpp"
#include "FEA/Test/inc/Truss3D.hpp"
#include "FEA/Test/inc/Rigid2D.hpp"
#include "FEA/inc/Mesh/Shapes/Shape.hpp"

int main(void)
{
	try
	{
		test::rigid2D::spring_bending();
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}