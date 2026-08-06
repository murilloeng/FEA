//std
#include <stdexcept>

//gmsh
#include <gmsh.h>

//FEA
#include "FEA/inc/Geometry/Surface.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Mesh/Elements/Type.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Surface::Surface(void) : m_structured{false}, m_element_type{mesh::elements::Type::Last}
		{
			return;
		}

		//destructor
		Surface::~Surface(void)
		{
			return;
		}

		//serialization
		void Surface::save(FILE* file) const
		{
			const uint32_t nl = m_loops.size();
			fprintf(file, "%d %02d ", m_structured, nl);
			for(const Loop& loop : m_loops) loop.save(file);
		}

		//data
		Geometry* Surface::geometry(void)
		{
			return m_geometry;
		}

		bool Surface::structured(void) const
		{
			return m_structured;
		}
		bool Surface::structured(bool structured)
		{
			return m_structured = structured;
		}
		
		const std::vector<uint32_t>& Surface::nodes(void) const
		{
			return m_nodes;
		}
		const std::vector<uint32_t>& Surface::elements(void) const
		{
			return m_elements;
		}

		mesh::elements::Type Surface::element_type(void) const
		{
			return m_element_type;
		}
		mesh::elements::Type Surface::element_type(mesh::elements::Type element_type)
		{
			return m_element_type = element_type;
		}

		//loops
		Loop& Surface::create_loop(void)
		{
			m_loops.push_back(Loop());
			return m_loops.back();
		}
		Loop& Surface::loop(uint32_t index)
		{
			if(index >= m_loops.size())
			{
				throw std::runtime_error("Surface loop's index is out of range!");
			}
			return m_loops[index];
		}
		const std::vector<Loop>& Surface::loops(void) const
		{
			return m_loops;
		}

		//index
		uint32_t Surface::index(void) const
		{
			return m_index;
		}

		//analysis
		void Surface::check(void) const
		{
			for(const Loop& loop : m_loops)
			{
				for(const Item& item : loop.items())
				{
					if(item.index() >= m_geometry->curves().size())
					{
						throw std::runtime_error("Error: Surface with out of range curve!");
					}
				}
			}
		}

		//mesh
		void Surface::mesh(void) const
		{
			return;
		}

		//static
		Geometry* Surface::m_geometry = nullptr;
	}
}