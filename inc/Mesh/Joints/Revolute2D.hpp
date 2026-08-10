#pragma once

//FEA
#include "FEA/inc/Mesh/Joints/Joint.hpp"

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			class Revolute2D : public Joint
			{
			public:
				//constructors
				Revolute2D(void);

				//destructor
				~Revolute2D(void);

			protected:
				//analysis
				void create_constraints(void) const override;
				void create_dependencies(void) const override;

				//draw
				void draw_setup(draw::Data&) const override;
				void draw_update(draw::Data&) const override;
			};
		}
	}
}