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

				//structs
				struct Data
				{
					math::Mat2 m_Kp;
					math::Mat3 m_Ks;
					math::Mat6 m_Ke;
					math::Vec6 m_fe;
					math::Vec2 m_ep, m_sp;
					math::Vec3 m_es, m_ss;
					math::Vec2 m_an, m_bn;
					math::Matrix<3, 4> m_L;
					math::Matrix<4, 6> m_N;
					math::Matrix<4, 4> m_dL;
					math::Matrix<2, 3> m_Ap;
				};
				

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
				void compute(analysis::Data&) override;

				//compute
				void compute_N(Data&) const;
				void compute_L(Data&) const;
				void compute_dL(Data&) const;
				void compute_ss(Data&) const;

				//data
				double m_q0;
				math::Vec2 m_a0;
				math::Mat3 m_Ks;
				double m_curvature;
				static bool m_large_strains;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}