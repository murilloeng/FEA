//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//Test
#include "FEA/Test/inc/Truss2D.hpp"
#include "FEA/Test/inc/Truss3D.hpp"

int main(void)
{
	try
	{
		test::truss3D::pyramid();
		// test::truss2D::von_mises();
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}