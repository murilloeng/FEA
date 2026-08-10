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

				//types
				typedef std::function<double(double)> Function;

			public:
				//data
				double value(double);
				double value(void) const;

				Function time_function(Function);
				Function time_function(void) const;
				double time_function(double) const;

			protected:
				//analysis
				virtual void check(void);
				virtual void setup(void);

				//data
				double m_value;
				Function m_time_function;
				static Boundary* m_boundary;

				//friends
				friend class fea::boundary::Boundary;
				friend class fea::boundary::LoadCase;
			};
		}
	}
}