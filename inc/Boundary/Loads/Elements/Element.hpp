#pragma once

//std
#include <vector>

//FEA
#include "FEA/inc/Boundary/Loads/Load.hpp"

#include "FEA/inc/Mesh/Elements/Element.hpp"

namespace fea
{
	namespace boundary
	{
		class Boundary;
		namespace loads
		{
			class LoadCase;
			enum class Type : uint32_t;
		}
	}
	namespace analysis
	{
		class Assembler;
	}
}

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Element : public Load
			{
			protected:
				//constructors
				Element(LoadCase*);

				//destructor
				virtual ~Element(void);

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

				//create
				static void create(Element*&, Type, LoadCase*);

			public:
				//type
				virtual Type type(void) const = 0;

				//index
				uint32_t index(void) const;
				uint32_t index_element(void) const;

				//name
				const char* type_name(void) const;
				static const char* type_name(Type);

				//data
				mesh::elements::Element* element(uint32_t);
				mesh::elements::Element* element(void) const;

			protected:
				//analysis
				virtual void prepare(void) override;
				virtual bool check(void) const override;

				//data
				uint32_t m_index;
				uint32_t m_element;

				//friends
				friend class LoadCase;
				friend class boundary::Boundary;
				friend class analysis::Assembler;
			};
		}
	}
}