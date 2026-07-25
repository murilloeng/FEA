#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Beam.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Beam3D : public Beam
			{
			public:
				//constructor
				Beam3D(void);

				//destructor
				~Beam3D(void);
			};
		}
	}
}