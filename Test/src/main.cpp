//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//Test
#include "FEA/Test/inc/Beam2D.hpp"
#include "FEA/Test/inc/Beam3D.hpp"

int main(void)
{
	try
	{
		test::beam3D::inelastic::pinned_force();
		test::beam3D::inelastic::pinned_bending();
		test::beam3D::inelastic::cantilever_force();
		test::beam3D::inelastic::cantilever_bending();
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}