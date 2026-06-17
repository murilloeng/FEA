#pragma once

//std
#include <cstdio>

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			enum class Type : uint32_t;
		}
		class Boundary;
	}
}

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			class Time
			{
			protected:
				//constructor
				Time(void);

				//destructor
				virtual ~Time(void);

				//serialization
				virtual void load(FILE*);
				virtual void save(FILE*) const;

				//create
				static void create(Time*&, const Time*);
				static void create(Time*&, time::Type, const Time* = nullptr);

			public:
				//data
				const char* label(void) const;
				const char* label(const char*);

				static Boundary* boundary(void);

				//type
				virtual time::Type type(void) const = 0;

				//name
				const char* type_name(void) const;
				static const char* type_name(time::Type);

				//index
				uint32_t index(void) const;

				//value
				virtual double value(double) const = 0;

			protected:
				//data
				uint32_t m_index;
				char m_label[200];
				static Boundary* m_boundary;

				//friends
				friend class boundary::Boundary;
			};
		}
	}
}