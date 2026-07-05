//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Truss::Truss(void)
			{
				return;
			}
			
			//destructor
			Truss::~Truss(void)
			{
				return;
			}

			//analysis
			void Truss::setup(void)
			{
				//data
				const math::Vec3 z1 = node(0)->m_position_ref;
				const math::Vec3 z2 = node(1)->m_position_ref;
				//length
				m_Lr = (z2 - z1).norm();
			}
			void Truss::compute(void)
			{
				//data
				const double A = m_section->area();
				const math::Vec3 x1 = node(0)->m_position_new;
				const math::Vec3 x2 = node(1)->m_position_new;
				const double E = m_material->elastic_modulus();
				//strain
				m_Ln = (x2 - x1).norm();
				m_em = strain_measure(m_strain_measure, m_Ln / m_Lr);
				m_eh = strain_hessian(m_strain_measure, m_Ln / m_Lr);
				m_eg = strain_gradient(m_strain_measure, m_Ln / m_Lr);
				//material
				const double C = E;
				const double s = E * m_em + m_sr;
				//compute
				m_f = s * A * m_eg;
				m_K = (C * m_eg * m_eg + s * m_eh) * A / m_Lr;
			}
		}
	}
}