#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Beam.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Beam2D : public Beam
			{
			public:
				//constructor
				Beam2D(void);

				//destructor
				~Beam2D(void);
			};
		}
	}
}