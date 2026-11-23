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
			class Beam3D : public Beam
			{
			private:
				//constructor
				Beam3D(void);

				//destructor
				~Beam3D(void) = default;

				//structs
				struct Data
				{
					math::Mat3 m_Kp;
					math::Mat6 m_Ks;
					math::Vec3 m_dx, m_pr;
					math::Vec3 m_ep, m_sp;
					math::Vec6 m_es, m_ss;
					math::Mat3 m_R1, m_Rm;
					math::Mat3 m_Ar, m_Hr;
					math::Vec3 m_t[2];
					math::Mat3 m_Q[2], m_J[2], m_T[2];
					math::Vector<12> m_fe;
					math::Matrix<3, 6> m_Ap;
					math::Matrix<6, 12> m_B;
					math::Matrix<12, 12> m_G;
					math::Matrix<12, 12> m_Ke;
				};

			public:
				//data
				math::Vec3& major_axis(void);
				const math::Vec3& major_axis(void) const;

			private:
				//data
				uint32_t stress_set(void) const override;
				uint32_t dof_set(uint32_t) const override;

				//analysis
				void check(void) override;
				void setup(void) override;
				void compute(analysis::Data&) override;

				//compute
				void compute_B(Data&) const;
				void compute_G(Data&) const;
				void compute_T(Data&) const;
				void compute_ss(Data&);

				//data
				math::Mat3 m_Qr;
				math::Mat6 m_Ks;
				math::Vec3 m_major_axis;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}
