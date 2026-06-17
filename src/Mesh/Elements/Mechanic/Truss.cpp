//std
#include <cmath>
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Types.hpp"
#include "FEA/inc/Mesh/Elements/States.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/StrainMeasure.hpp"

#include "FEA/inc/Boundary/Loads/Types.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Element.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Line/LineForce.hpp"

#include "FEA/inc/Results/What.hpp"
#include "FEA/inc/Results/Results.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructors
			Truss::Truss(void) : m_tension{false}, m_f{0}, m_K{0}, m_sr{0}
			{
				return;
			}

			//destructor
			Truss::~Truss(void)
			{
				return;
			}

			//serialization
			void Truss::load(FILE* file)
			{
				uint32_t tension;
				Frame::load(file);
				if(fscanf(file, "%d %lf", &tension, &m_sr) != 2)
				{
					throw std::runtime_error("Error: Truss element loading failed!");
				}
				m_tension = tension;
			}
			void Truss::save(FILE* file) const
			{
				Frame::save(file);
				if(fprintf(file, "%d %+.6e ", m_tension, m_sr) < 0)
				{
					throw std::runtime_error("Error: Truss element saving failed!");
				}
			}

			//types
			uint32_t Truss::loads_set(void) const
			{
				return (uint32_t) boundary::loads::Type::LineForce;
			}
			uint64_t Truss::states_set(void) const
			{
				return (uint64_t) elements::State::force_1;
			}

			//data
			bool Truss::tension(void) const
			{
				return m_tension;
			}
			bool Truss::tension(bool tension)
			{
				return m_tension = tension;
			}

			double Truss::residual_stress(void) const
			{
				return m_sr;
			}
			double Truss::residual_stress(double sr)
			{
				return m_sr = sr;
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
			void Truss::record(void)
			{
				Frame::record();
				if(m_mesh->model()->results()->what()->elements())
				{
					const uint32_t s = m_mesh->step();
					m_state_data[2 * s + 0] = m_state_data[2 * s + 1] = m_f;
				}
			}
			void Truss::compute(void)
			{
				//kinematics
				math::Vec3 x1, x2;
				node(0)->position(x1.data());
				node(1)->position(x2.data());
				const math::Vec3 z1 = node(0)->coordinates();
				const math::Vec3 z2 = node(1)->coordinates();
				//length
				const double ln = (x2 - x1).norm();
				const double lr = (z2 - z1).norm();
				//direction
				const math::Vec3 s1 = (z2 - z1) / lr;
				//section
				const double A = m_section->area();
				//material
				const double E = m_material->elastic_modulus();
				//strain
				const double e = strain_measure(ln / lr);
				const double he = strain_hessian(ln / lr);
				const double ge = strain_gradient(ln / lr);
				//return mapping
				const double C = E;
				const double s = E * e + m_sr;
				//local force and stiffness
				m_f = m_tension && s < 0 ? 0 : s * ge * A;
				m_K = m_tension && s < 0 ? 0 : (C * ge * ge + s * he) * A / lr;
			}

			//strain
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

			//loads
			void Truss::reference_force(double* f, const boundary::loads::Element* load) const
			{
				if(load->type() == boundary::loads::Type::LineForce)
				{
					load_line_force(f, (const boundary::loads::LineForce*) load);
				}
				else
				{
					Frame::reference_force(f, load);
				}
			}

			StrainMeasure Truss::m_strain_measure = StrainMeasure::Logarithmic;
		}
	}
}