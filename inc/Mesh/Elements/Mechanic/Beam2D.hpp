#pragma once

//Math
#include "Math/inc/Linear/Matrix.hpp"

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

				//types
				typedef math::Vector<2> vec2;
				typedef math::Vector<3> vec3;

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

				void compute_ss(vec3&, const vec3&);

				void compute_N(math::Matrix<4, 6>&) const;
				void compute_L(math::Matrix<3, 4>&, const vec2&, const vec2&, const vec3&) const;
				void compute_dL(math::Matrix<4, 4>&, const vec2&, const vec2&, const vec3&, const vec3&) const;

				//data
				double m_q0;
				double m_curvature;
				math::Vector<2> m_a0;
				static bool m_large_strains;
				math::Matrix<3, 3> m_Ks;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}