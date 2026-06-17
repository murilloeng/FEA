//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

int main(void)
{
	try
	{
		return 0;
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}