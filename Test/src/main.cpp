//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

int main(void)
{
	try
	{
		//data
		fea::Model model;
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}