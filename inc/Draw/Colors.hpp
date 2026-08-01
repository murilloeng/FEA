#pragma once

//Canvas
#include "Canvas/inc/Colors/Color.hpp"

namespace fea
{
	namespace draw
	{
		class Colors
		{
		public:
			//constructors
			Colors(void);

			//destructor
			~Colors(void);

			//data
			canvas::Color text(void) const;
			canvas::Color text(canvas::Color);
			canvas::Color text(float, float, float, float);

			canvas::Color nodes(void) const;
			canvas::Color nodes(canvas::Color);
			canvas::Color nodes(float, float, float, float);

			canvas::Color loads(void) const;
			canvas::Color loads(canvas::Color);
			canvas::Color loads(float, float, float, float);

			canvas::Color paths(void) const;
			canvas::Color paths(canvas::Color);
			canvas::Color paths(float, float, float, float);

			canvas::Color joints(void) const;
			canvas::Color joints(canvas::Color);
			canvas::Color joints(float, float, float, float);

			canvas::Color points(void) const;
			canvas::Color points(canvas::Color);
			canvas::Color points(float, float, float, float);

			canvas::Color curves(void) const;
			canvas::Color curves(canvas::Color);
			canvas::Color curves(float, float, float, float);

			canvas::Color elements(void) const;
			canvas::Color elements(canvas::Color);
			canvas::Color elements(float, float, float, float);

			canvas::Color supports(void) const;
			canvas::Color supports(canvas::Color);
			canvas::Color supports(float, float, float, float);

			canvas::Color surfaces(void) const;
			canvas::Color surfaces(canvas::Color);
			canvas::Color surfaces(float, float, float, float);

			canvas::Color background(void) const;
			canvas::Color background(canvas::Color);
			canvas::Color background(float, float, float, float);

		private:
			//data
			canvas::Color m_nodes;
			canvas::Color m_joints;
			canvas::Color m_elements;
			
			canvas::Color m_paths;
			canvas::Color m_loads;
			canvas::Color m_supports;

			canvas::Color m_points;
			canvas::Color m_curves;
			canvas::Color m_surfaces;
			
			canvas::Color m_text;
			canvas::Color m_background;
		};
	}
}