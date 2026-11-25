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
					math::Mat6 m_Ks;
					math::Vec6 m_es, m_ss;
					math::Vec3 m_ep, m_sp, m_tr, m_xr;
					math::Mat3 m_Kp, m_R1, m_Ji, m_Gi, m_T1, m_T2;
					math::Matrix<3, 6> m_A;
					math::Matrix<6, 12> m_B;
					math::Matrix<12, 12> m_G;
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
				void compute_shear(Data&);
				void compute_section(Data&);
				void compute_strains_hessian(Data&);
				void compute_strains_gradient(Data&);

				//data
				math::Mat3 m_Rr;
				math::Mat6 m_Ks;
				math::Vec3 m_major_axis;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}
