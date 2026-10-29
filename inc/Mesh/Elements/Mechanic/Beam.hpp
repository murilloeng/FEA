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

				static uint32_t draw_mesh(void);
				static uint32_t draw_mesh(uint32_t);

				math::quadrature::Quadrature& quadrature(void);

				const double* major_axis(void) const;
				const double* major_axis(const double*);
				const double* major_axis(double, double, double);

			protected:
				//analysis
				void check(void) override;
				void setup(void) override;
				void update(void) override;
				void restore(void) override;

				//data
				static bool m_shear;
				static bool m_mixed;
				double m_major_axis[3];
				static uint32_t m_draw_mesh;
				std::vector<points::Section> m_points;
				math::quadrature::Quadrature m_quadrature;
			};
		}
	}
}