//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//Test
#include "FEA/Test/inc/Beam2D.hpp"
#include "FEA/Test/inc/Truss2D.hpp"
#include "FEA/Test/inc/Truss3D.hpp"
#include "FEA/Test/inc/Rigid2D.hpp"

int main(void)
{
	try
	{
		test::rigid2D::spring_buckling();
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}