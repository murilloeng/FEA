#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Frame.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Beam : public Frame
			{
			public:
				//constructor
				Beam(void);

				//destructor
				~Beam(void);

				//data
				bool shear(bool);
				bool shear(void) const;

				bool mixed(bool);
				bool mixed(void) const;

			protected:
				//data
				bool m_shear;
				bool m_mixed;
			};
		}
	}
}