//std
#include <stdexcept>

//gmsh
#include <gmsh.h>

//FEA
#include "FEA/inc/Geometry/Surface.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Surface::Surface(void) : m_structured{false}
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

		Surface::function Surface::generate_elements(void) const
		{
			return m_generate_elements;
		}
		Surface::function Surface::generate_elements(function generate_elements)
		{
			return m_generate_elements = generate_elements;
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

		//mesh
		void Surface::mesh(void) const
		{
			return;
		}

		//static
		Geometry* Surface::m_geometry = nullptr;
	}
}