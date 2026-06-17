#pragma once

//std
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			class Time;
		}
		namespace loads
		{
			class LoadCase;
		}
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
				//constructors
				Load(LoadCase*);

				//destructor
				virtual ~Load(void);

				//serialization
				virtual void load(FILE*);
				virtual void save(FILE*) const;

			public:
				//data
				double value(double);
				double value(void) const;

				time::Time* time(uint32_t);
				time::Time* time(void) const;

				bool draw_direction(bool);
				bool draw_direction(void) const;

				double time_value(double) const;

				LoadCase* load_case(void) const;

				//index
				uint32_t index_time(void) const;

			protected:
				//analysis
				virtual void prepare(void) = 0;
				virtual bool check(void) const = 0;

				//data
				double m_value;
				uint32_t m_time;
				bool m_draw_direction;
				LoadCase* const m_load_case;

				//friends
				friend class LoadCase;
			};
		}
	}
}