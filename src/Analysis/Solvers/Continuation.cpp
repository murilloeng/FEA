//std
#include <cmath>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/util.hpp"
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Analysis/Analysis.hpp"

#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Solvers/Continuation.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructor
			Continuation::Continuation(Nonlinear* solver) : m_type{Type::MinimalNorm}, m_solver{solver}
			{
				return;
			}

			//destructor
			Continuation::~Continuation(void)
			{
				return;
			}

			//serialization
			void Continuation::load(FILE* file)
			{
				uint32_t type;
				if(fscanf(file, "%d", &type) != 1)
				{
					throw std::runtime_error("Continuation loading error!");
				}
				m_type = Type(type);
			}
			void Continuation::save(FILE* file) const
			{
				if(fprintf(file, "%d ", uint32_t(m_type)) < 0)
				{
					throw std::runtime_error("Continuation saving error!");
				}
			}

			//data
			Continuation::Type Continuation::type(void) const
			{
				return m_type;
			}
			Continuation::Type Continuation::type(Continuation::Type type)
			{
				return m_type = type;
			}

			//continuation
			double Continuation::predictor(void) const
			{
				double(Continuation::*predictors[])(void) const = {
					&Continuation::predictor_minimal_norm,
					&Continuation::predictor_control_load,
					&Continuation::predictor_control_state,
					&Continuation::predictor_arc_length_spherical,
					&Continuation::predictor_arc_length_cylindrical
				};
				return (this->*predictors[uint32_t(m_type)])();
			}
			double Continuation::corrector(void) const
			{
				double(Continuation::*correctors[])(void) const = {
					&Continuation::corrector_minimal_norm,
					&Continuation::corrector_control_load,
					&Continuation::corrector_control_state,
					&Continuation::corrector_arc_length_spherical,
					&Continuation::corrector_arc_length_cylindrical
				};
				return (this->*correctors[uint32_t(m_type)])();
			}

			//type
			const char* Continuation::type_name(void) const
			{
				return type_name(m_type);
			}
			const char* Continuation::type_name(Type type)
			{
				//data
				const char* names[] = {
					"Minimal norm",
					"Control load",
					"Control state",
					"Arc length spherical",
					"Arc length cylindrical"
				};
				return names[uint32_t(type)];
			}

			//size
			uint32_t Continuation::size(void) const
			{
				return m_solver->analysis()->assembler()->dof_unknow();
			}

			//continuation
			double Continuation::predictor_minimal_norm(void) const
			{
				//data
				const uint32_t nd = size();
				const math::Vector dx(m_solver->m_dx, nd);
				const math::Vector dxr(m_solver->m_dxr, nd);
				const math::Vector dxt(m_solver->m_dxt, nd);
				//predictor
				return (dx - dxr).inner(dxt) / dxt.inner(dxt);
			}
			double Continuation::corrector_minimal_norm(void) const
			{
				//data
				const uint32_t nd = size();
				const math::Vector ddxr(m_solver->m_ddxr, nd);
				const math::Vector ddxt(m_solver->m_ddxt, nd);
				//corrector
				return -ddxr.inner(ddxt) / ddxt.inner(ddxt);
			}
			double Continuation::predictor_control_load(void) const
			{
				return m_solver->m_dp;
			}
			double Continuation::corrector_control_load(void) const
			{
				return 0;
			}
			double Continuation::predictor_control_state(void) const
			{
				const uint32_t index = m_solver->m_watch_dof.index_dof();
				return (m_solver->m_dx[index] - m_solver->m_dxr[index]) / m_solver->m_dxt[index];
			}
			double Continuation::corrector_control_state(void) const
			{
				const uint32_t index = m_solver->m_watch_dof.index_dof();
				return -m_solver->m_ddxr[index] / m_solver->m_ddxt[index];
			}
			double Continuation::predictor_arc_length_spherical(void) const
			{
				//data
				const uint32_t nd = size();
				const double dp = m_solver->m_dp;
				const math::Vector dx(m_solver->m_dx, nd);
				const math::Vector dxr(m_solver->m_dxr, nd);
				const math::Vector dxt(m_solver->m_dxt, nd);
				//data
				const double b = dxt.inner(dxr);
				const double a = dxt.inner(dxt) + 1;
				const double s = math::sign(dxt.inner(dx));
				const double c = dxr.inner(dxr) - dx.inner(dx) - dp * dp;
				//predictor
				return -b / a + s * sqrt(b * b - c * a) / a;
			}
			double Continuation::corrector_arc_length_spherical(void) const
			{
				//data
				const uint32_t nd = size();
				const double dp = m_solver->m_dp;
				const math::Vector dx(m_solver->m_dx, nd);
				const math::Vector ddxr(m_solver->m_ddxr, nd);
				const math::Vector ddxt(m_solver->m_ddxt, nd);
				//data
				const double a = ddxt.inner(ddxt) + 1;
				const double c = ddxr.inner(ddxr + 2 * dx);
				const double b = ddxt.inner(ddxr + dx) + dp;
				const double s = math::sign(ddxt.inner(dx));
				//predictor
				return -b / a + s * sqrt(b * b - c * a) / a;
			}
			double Continuation::predictor_arc_length_cylindrical(void) const
			{
				//data
				const uint32_t nd = size();
				const math::Vector dx(m_solver->m_dx, nd);
				const math::Vector dxr(m_solver->m_dxr, nd);
				const math::Vector dxt(m_solver->m_dxt, nd);
				//data
				const double a = dxt.inner(dxt);
				const double b = dxt.inner(dxr);
				const double s = math::sign(dxt.inner(dx));
				const double c = dxr.inner(dxr) - dx.inner(dx);
				//predictor
				return -b / a + s * sqrt(b * b - c * a) / a;
			}
			double Continuation::corrector_arc_length_cylindrical(void) const
			{
				//data
				const uint32_t nd = size();
				const math::Vector dx(m_solver->m_dx, nd);
				const math::Vector ddxr(m_solver->m_ddxr, nd);
				const math::Vector ddxt(m_solver->m_ddxt, nd);
				//data
				const double a = ddxt.inner(ddxt);
				const double b = ddxt.inner(ddxr + dx);
				const double c = ddxr.inner(ddxr + 2 * dx);
				const double s = math::sign(ddxt.inner(dx));
				//predictor
				return -b / a + s * sqrt(b * b - c * a) / a;
			}
		}
	}
}