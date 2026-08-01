#pragma once

namespace fea
{
	namespace draw
	{
		class Sizes
		{
		public:
			//constructors
			Sizes(void);
	
			//destructor
			~Sizes(void);
	
			//data
			float loads(float);
			float joints(float);
			float curves(float);
			float graphs(float);
			float elements(float);
			float supports(float);
	
			float loads(void) const;
			float joints(void) const;
			float curves(void) const;
			float graphs(void) const;
			float elements(void) const;
			float supports(void) const;
	
		private:
			//data
			float m_loads;
			float m_joints;
			float m_curves;
			float m_graphs;
			float m_elements;
			float m_supports;
		};
	}
}