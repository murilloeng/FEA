#pragma once

//std
#include <vector>
#include <cstdint>

//FEA
#include "FEA/inc/Mesh/Elements/Type.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Element
			{
			public:
				//constructor
				Element(void);

				//destructor
				~Element(void);
				
				//type
				virtual Type type(void) const = 0;

				//dof
				virtual uint32_t dof(uint32_t) const = 0;

				//data
				uint32_t node(uint32_t) const;
				uint32_t node(uint32_t, uint32_t);
				const std::vector<uint32_t>& nodes(void) const;

				//tangents
				virtual void inertia(double*) const = 0;
				virtual void damping(double*) const = 0;
				virtual void stiffness(double*) const = 0;

				//forces
				virtual void internal_force(double*) const = 0;

			private:
				//data
				std::vector<uint32_t> m_nodes;
			};
		}
	}
}