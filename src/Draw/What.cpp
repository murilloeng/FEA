//FEA
#include "FEA/inc/Draw/What.hpp"

namespace fea
{
	namespace draw
	{
		//constructors
		What::What(void) : 
			m_nodes{true}, m_loads{true}, m_graph{false}, m_joints{true}, m_volume{true}, m_points{true},
			m_curves{true}, m_elements{true}, m_supports{true}, m_surfaces{true}, m_deformed{true}, m_load_case{0}
		{
			return;
		}
	
		//destructor
		What::~What(void)
		{
			return;
		}

		//data
		bool What::nodes(void) const
		{
			return m_nodes;
		}
		bool What::nodes(bool nodes)
		{
			return m_nodes = nodes;
		}

		bool What::loads(void) const
		{
			return m_loads;
		}
		bool What::loads(bool loads)
		{
			return m_loads = loads;
		}

		bool What::graph(void) const
		{
			return m_graph;
		}
		bool What::graph(bool graph)
		{
			return m_graph = graph;
		}

		bool What::joints(void) const
		{
			return m_joints;
		}
		bool What::joints(bool joints)
		{
			return m_joints = joints;
		}

		bool What::volume(void) const
		{
			return m_volume;
		}
		bool What::volume(bool volume)
		{
			return m_volume = volume;
		}

		bool What::points(void) const
		{
			return m_points;
		}
		bool What::points(bool points)
		{
			return m_points = points;
		}

		bool What::curves(void) const
		{
			return m_curves;
		}
		bool What::curves(bool curves)
		{
			return m_curves = curves;
		}

		bool What::elements(void) const
		{
			return m_elements;
		}
		bool What::elements(bool elements)
		{
			return m_elements = elements;
		}

		bool What::supports(void) const
		{
			return m_supports;
		}
		bool What::supports(bool supports)
		{
			return m_supports = supports;
		}

		bool What::surfaces(void) const
		{
			return m_surfaces;
		}
		bool What::surfaces(bool surfaces)
		{
			return m_surfaces = surfaces;
		}

		bool What::deformed(void) const
		{
			return m_deformed;
		}
		bool What::deformed(bool deformed)
		{
			return m_deformed = deformed;
		}

		uint32_t What::load_case(void) const
		{
			return m_load_case;
		
		}
		uint32_t What::load_case(uint32_t group)
		{
			return m_load_case = group;
		}
	}
}