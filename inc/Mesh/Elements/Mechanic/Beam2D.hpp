#pragma once

//Math
#include "Math/inc/Linear/Mat3.hpp"

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
			private:
				//constructor
				Beam2D(void);

				//destructor
				~Beam2D(void) = default;

			public:
				//data
				double curvature(double);
				double curvature(void) const;

				static bool large_strains(void);
				static bool large_strains(bool);

			private:
				//data
				uint32_t stress_set(void) const override;
				uint32_t dof_set(uint32_t) const override;

				//draw
				void draw_setup(draw::Data&) const override;
				void draw_update(draw::Data&) const override;

				//analysis
				void setup(void) override;
				void compute(analysis::Compute&) override;

				void internal_force(double* f) const;

				//data
				math::Mat3 m_Ks;
				double m_curvature;
				static bool m_large_strains;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}