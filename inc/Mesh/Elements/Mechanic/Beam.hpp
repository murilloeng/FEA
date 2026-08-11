#pragma once

//Math
#include "Math/inc/Quadrature/Quadrature.hpp"

//FEA
#include "FEA/inc/Mesh/Points/Section.hpp"
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
				static bool shear(bool);
				static bool shear(void);

				static bool mixed(bool);
				static bool mixed(void);

				math::quadrature::Quadrature& quadrature(void);

			protected:
				//analysis
				void setup(void) override;
				void update(void) override;
				void restore(void) override;

				//data
				static bool m_shear;
				static bool m_mixed;
				std::vector<points::Section> m_points;
				math::quadrature::Quadrature m_quadrature;
			};
		}
	}
}