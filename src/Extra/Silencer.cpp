//std
#include <stdexcept>

//FEA
#include "FEA/inc/Extra/Silencer.hpp"

#if defined(_WIN32)
	static const char* stream_null = "NUL"
	static const char* stream_main = "CONOUT$"
#else
	static const char* stream_main = "/dev/tty";
	static const char* stream_null = "/dev/null";
#endif

namespace fea
{
	namespace extra
	{
		//constructor
		Silencer::Silencer(void)
		{
			fflush(stdout);
			if(!freopen(stream_null, "w", stdout))
			{
				throw std::runtime_error("Silencer redirection error!");
			}
		}

		//destructor
		Silencer::~Silencer(void)
		{
			fflush(stdout);
			if(!freopen(stream_main, "w", stdout))
			{
				printf("Silencer redirection error!");
			}
		}
	}
}