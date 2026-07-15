//std
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vec3.hpp"

//Materials
#include "Materials/inc/Mechanic/Stress.hpp"
#include "Materials/inc/Mechanic/Uniaxial.hpp"

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

			//save
			void Truss::save(FILE* file) const
			{
				Frame::save(file);
				fprintf(file, "Residual stress: %+.6e Strain measure: %d ", m_sr, uint32_t(m_strain_measure));
			}

			//data
			double Truss::residual_stress(double sr)
			{
				return m_sr = sr;
			}
			double Truss::residual_stress(void) const
			{
				return m_sr;
			}

			StrainMeasure Truss::strain_measure(void)
			{
				return m_strain_measure;
			}
			StrainMeasure Truss::strain_measure(StrainMeasure strain_measure)
			{
				return m_strain_measure = strain_measure;
			}

			//analysis
			void Truss::check(void)
			{
				Frame::check();
				if(!dynamic_cast<const materials::Uniaxial*>(m_material))
				{
					throw std::runtime_error("Error: Truss element must have an Uniaxial material!");
				}
			}
			void Truss::setup(void)
			{
				//data
				Frame::setup();
				const math::Vec3 z1 = node(0)->position_ref();
				const math::Vec3 z2 = node(1)->position_ref();
				//length
				m_Lr = (z2 - z1).norm();
				//point
				m_point.prepare(uint32_t(materials::Stress::type::s11));
			}
			void Truss::compute(void)
			{
				//data
				const double A = m_section->area();
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//strain
				m_Ln = (x2 - x1).norm();
				m_em = elements::strain_measure(m_strain_measure, m_Ln / m_Lr);
				m_eh = elements::strain_hessian(m_strain_measure, m_Ln / m_Lr);
				m_eg = elements::strain_gradient(m_strain_measure, m_Ln / m_Lr);
				//material
				double s, C;
				((materials::Uniaxial*) m_material)->return_mapping(s, C, m_em, m_sr, m_point);
				//compute
				m_f = s * A * m_eg;
				m_K = (C * m_eg * m_eg + s * m_eh) * A / m_Lr;
			}

			//static
			StrainMeasure Truss::m_strain_measure = StrainMeasure::Linear;
		}
	}
}