#pragma once

//std
#include <cstdio>
#include <functional>

namespace fea
{
	namespace boundary
	{
		class Boundary;
		class LoadCase;
	}
}

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Load
			{
			protected:
				//constructor
				Load(void);

				//destructor
				virtual ~Load(void);

				//serialization
				virtual void save(FILE*) const;

			public:
				//data
				double value(double);
				double value(void) const;

				std::function<double(double)> function(void) const;
				std::function<double(double)> function(std::function<double(double)>);

			protected:
				//analysis
				virtual void check(void);
				virtual void setup(void);

				//data
				double m_value;
				static Boundary* m_boundary;
				std::function<double(double)> m_function;

				//friends
				friend class fea::boundary::Boundary;
				friend class fea::boundary::LoadCase;
			};
		}
	}
}