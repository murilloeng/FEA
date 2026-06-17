//std
#include <cmath>

//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Truss.hpp"

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

			//data
			StrainMeasure Truss::strain_measure(void)
			{
				return m_strain_measure;
			}
			StrainMeasure Truss::strain_measure(StrainMeasure strain_measure)
			{
				return m_strain_measure = strain_measure;
			}

			//analysis
			void Truss::compute(void)
			{
				//kinematics
				const math::Vec3 x0 = node(0)->position(true);
				const math::Vec3 x1 = node(1)->position(true);
				const math::Vec3 z0 = node(0)->position(false);
				const math::Vec3 z1 = node(1)->position(false);
				//length
				const double Ln = (x1 - x0).norm();
				const double Lr = (z1 - z0).norm();
				//section
				const double A = m_section->area();
				//material
				const double E = m_material->elastic_modulus();
				//strain
				const double e = strain_measure(Ln / Lr);
				const double he = strain_hessian(Ln / Lr);
				const double ge = strain_gradient(Ln / Lr);
				//return mapping
				const double C = E;
				const double s = E * e;
				//compute
				m_f = s * ge * A;
				m_K = (C * ge * ge + s * he) * A / Lr;
			}

			//strains
			double Truss::strain_measure(double s)
			{
				return
					m_strain_measure == StrainMeasure::Linear ? s - 1 :
					m_strain_measure == StrainMeasure::Quadratic ? (s * s - 1) / 2 :
					m_strain_measure == StrainMeasure::Logarithmic ? log(s) : 0;
			}
			double Truss::strain_hessian(double s)
			{
				return
					m_strain_measure == StrainMeasure::Linear ? 0 :
					m_strain_measure == StrainMeasure::Quadratic ? 1 :
					m_strain_measure == StrainMeasure::Logarithmic ? -1 / s / s : 0;
			}
			double Truss::strain_gradient(double s)
			{
				return
					m_strain_measure == StrainMeasure::Linear ? 1 :
					m_strain_measure == StrainMeasure::Quadratic ? s :
					m_strain_measure == StrainMeasure::Logarithmic ? 1 / s : 0;
			}

			//static data
			StrainMeasure Truss::m_strain_measure = StrainMeasure::Logarithmic;
		}
	}
}