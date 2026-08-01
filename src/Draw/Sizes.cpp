//FEA
#include "FEA/inc/Draw/Sizes.hpp"

namespace fea
{
	namespace draw
	{
		//constructors
		Sizes::Sizes(void) : 
			m_loads{1.00e-01}, m_joints{5.00e-02}, m_curves{3.00e-01}, m_graphs{2.00e-01}, m_elements{1.00e-01}, m_supports{5.00e-02}
		{
			return;
		}
	
		//destructor
		Sizes::~Sizes(void)
		{
			return;
		}
	
		//data
		float Sizes::loads(float loads)
		{
			return m_loads = loads;
		}
		float Sizes::joints(float joints)
		{
			return m_joints = joints;
		}
		float Sizes::curves(float curves)
		{
			return m_curves = curves;
		}
		float Sizes::graphs(float graphs)
		{
			return m_graphs = graphs;
		}
		float Sizes::elements(float elements)
		{
			return m_elements = elements;
		}
		float Sizes::supports(float supports)
		{
			return m_supports = supports;
		}
	
		float Sizes::loads(void) const
		{
			return m_loads;
		}
		float Sizes::joints(void) const
		{
			return m_joints;
		}
		float Sizes::curves(void) const
		{
			return m_curves;
		}
		float Sizes::graphs(void) const
		{
			return m_graphs;
		}
		float Sizes::supports(void) const
		{
			return m_supports;
		}
		float Sizes::elements(void) const
		{
			return m_elements;
		}
	}
}
