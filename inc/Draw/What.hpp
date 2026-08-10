#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace draw
	{
		class What
		{
		public:
			//constructors
			What(void);

			//destructor
			~What(void);

			//data
			bool nodes(bool);
			bool nodes(void) const;

			bool loads(bool);
			bool loads(void) const;

			bool graph(bool);
			bool graph(void) const;

			bool joints(bool);
			bool joints(void) const;

			bool volume(bool);
			bool volume(void) const;

			bool points(bool);
			bool points(void) const;

			bool curves(bool);
			bool curves(void) const;

			bool elements(bool);
			bool elements(void) const;

			bool supports(bool);
			bool supports(void) const;

			bool surfaces(bool);
			bool surfaces(void) const;

			bool deformed(bool);
			bool deformed(void) const;

			uint32_t load_case(uint32_t);
			uint32_t load_case(void) const;

		private:
			//data
			bool m_nodes;
			bool m_loads;
			bool m_graph;
			bool m_joints;
			bool m_volume;
			bool m_points;
			bool m_curves;
			bool m_elements;
			bool m_supports;
			bool m_surfaces;
			bool m_deformed;
			uint32_t m_load_case;
		};
	}
}