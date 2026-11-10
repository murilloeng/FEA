//std
#include <stdexcept>

//Materials
#include "Materials/inc/Mechanic/Stress.hpp"
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Truss.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Truss::Truss(void) : m_sr{0}
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
				Frame::setup();
				m_point.setup(1 << uint32_t(materials::Stress::Type::s11));
			}
			void Truss::update(void)
			{
				m_point.update();
			}
			void Truss::restore(void)
			{
				m_point.restore();
			}

			//static
			StrainMeasure Truss::m_strain_measure = StrainMeasure::Linear;
		}
	}
}