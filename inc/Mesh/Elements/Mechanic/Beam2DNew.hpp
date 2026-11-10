#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Beam.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Beam2DNew : public Beam
			{
			private:
				//constructor
				Beam2DNew(void);

				//destructor
				~Beam2DNew(void);

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
				void compute(analysis::Compute&) override;

				//data
				double m_curvature;
				static bool m_large_strains;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}