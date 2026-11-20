//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//Test
#include "FEA/Test/inc/Beam2D.hpp"
#include "FEA/Test/inc/Beam3D.hpp"
#include "FEA/Test/inc/Rigid3D.hpp"

int main(void)
{
	try
	{
		test::rigid3D::arm_bending();
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}