//std
#include <functional>

//FEA
#include "FEA/inc/Boundary/Time/Time.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			class Custom : public Time
			{
			private:
				//constructors
				Custom(void);

				//destructor
				~Custom(void);

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				std::function<double(double)> function(void) const;
				std::function<double(double)> function(std::function<double(double)>);

				//type
				time::Type type(void) const override;

				//value
				double value(double) const override;

			private:
				//data
				std::function<double(double)> m_function;

				//friends
				friend class Time;
			};
		}
	}
}