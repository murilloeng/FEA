//std
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <stdexcept>

//FEA
#include "FEA/inc/Draw/Colors.hpp"

namespace fea
{
	namespace draw
	{
		//constructors
		Colors::Colors(void) : 
			m_nodes{1.00, 0.00, 0.00, 1.00}, m_joints{0.00, 1.00, 0.00, 1.00}, m_elements{0.00, 0.00, 1.00, 1.00}, 
			m_paths{0.50, 0.50, 0.50, 1.00}, m_loads{1.00, 0.39, 0.28, 1.00}, m_supports{0.50, 0.50, 0.50, 1.00}, 
			m_points{1.00, 0.00, 0.00, 1.00}, m_curves{0.00, 1.00, 0.00, 1.00}, m_surfaces{0.00, 0.00, 1.00, 1.00}, 
			m_text{1.00, 1.00, 1.00, 1.00}, m_background{0.00, 0.00, 0.00, 1.00}
		{
			return;
		}

		//destructor
		Colors::~Colors(void)
		{
			return;
		}

		//data
		canvas::Color Colors::text(void) const
		{
			return m_text;
		}
		canvas::Color Colors::text(canvas::Color text)
		{
			return m_text = text;
		}
		canvas::Color Colors::text(float red, float green, float blue, float alpha)
		{
			return m_text = {red, green, blue, alpha};
		}

		canvas::Color Colors::nodes(void) const
		{
			return m_nodes;
		}
		canvas::Color Colors::nodes(canvas::Color nodes)
		{
			return m_nodes = nodes;
		}
		canvas::Color Colors::nodes(float red, float green, float blue, float alpha)
		{
			return m_nodes = {red, green, blue, alpha};
		}

		canvas::Color Colors::loads(void) const
		{
			return m_loads;
		}
		canvas::Color Colors::loads(canvas::Color loads)
		{
			return m_loads = loads;
		}
		canvas::Color Colors::loads(float red, float green, float blue, float alpha)
		{
			return m_loads = {red, green, blue, alpha};
		}

		canvas::Color Colors::paths(void) const
		{
			return m_paths;
		}
		canvas::Color Colors::paths(canvas::Color paths)
		{
			return m_paths = paths;
		}
		canvas::Color Colors::paths(float red, float green, float blue, float alpha)
		{
			return m_paths = {red, green, blue, alpha};
		}

		canvas::Color Colors::joints(void) const
		{
			return m_joints;
		}
		canvas::Color Colors::joints(canvas::Color joints)
		{
			return m_joints = joints;
		}
		canvas::Color Colors::joints(float red, float green, float blue, float alpha)
		{
			return m_joints = {red, green, blue, alpha};
		}

		canvas::Color Colors::points(void) const
		{
			return m_points;
		}
		canvas::Color Colors::points(canvas::Color points)
		{
			return m_points = points;
		}
		canvas::Color Colors::points(float red, float green, float blue, float alpha)
		{
			return m_points = {red, green, blue, alpha};
		}

		canvas::Color Colors::curves(void) const
		{
			return m_curves;
		}
		canvas::Color Colors::curves(canvas::Color curves)
		{
			return m_curves = curves;
		}
		canvas::Color Colors::curves(float red, float green, float blue, float alpha)
		{
			return m_curves = {red, green, blue, alpha};
		}

		canvas::Color Colors::elements(void) const
		{
			return m_elements;
		}
		canvas::Color Colors::elements(canvas::Color elements)
		{
			return m_elements = elements;
		}
		canvas::Color Colors::elements(float red, float green, float blue, float alpha)
		{
			return m_elements = {red, green, blue, alpha};
		}

		canvas::Color Colors::supports(void) const
		{
			return m_supports;
		}
		canvas::Color Colors::supports(canvas::Color supports)
		{
			return m_supports = supports;
		}
		canvas::Color Colors::supports(float red, float green, float blue, float alpha)
		{
			return m_supports = {red, green, blue, alpha};
		}

		canvas::Color Colors::surfaces(void) const
		{
			return m_surfaces;
		}
		canvas::Color Colors::surfaces(canvas::Color surfaces)
		{
			return m_surfaces = surfaces;
		}
		canvas::Color Colors::surfaces(float red, float green, float blue, float alpha)
		{
			return m_surfaces = {red, green, blue, alpha};
		}

		canvas::Color Colors::background(void) const
		{
			return m_background;
		}
		canvas::Color Colors::background(canvas::Color background)
		{
			return m_background = background;
		}
		canvas::Color Colors::background(float red, float green, float blue, float alpha)
		{
			return m_background = {red, green, blue, alpha};
		}
	}
}