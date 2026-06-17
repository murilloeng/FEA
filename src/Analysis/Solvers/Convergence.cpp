//std
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Time.hpp"
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"
#include "FEA/inc/Analysis/Solvers/Convergence.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructor
			Convergence::Convergence(Nonlinear* solver) : m_type(Type::LoadRelative), m_tolerance{1e-5}, m_solver{solver}
			{
				return;
			}

			//destructor
			Convergence::~Convergence(void)
			{
				return;
			}

			//serialization
			void Convergence::load(FILE* file)
			{
				uint32_t type;
				if(fscanf(file, "%d %lf", &type, &m_tolerance) != 2)
				{
					throw std::runtime_error("Convergence loading error!");
				}
				m_type = Type(type);
			}
			void Convergence::save(FILE* file) const
			{
				if(fprintf(file, "%d %+.6e ", uint32_t(m_type), m_tolerance) < 0)
				{
					throw std::runtime_error("Convergence saving error!");
				}
			}

			//compute
			bool Convergence::compute(void)
			{
				//data
				const double* r = m_solver->m_r;
				const double* dx = m_solver->m_dx;
				const double* fe = m_solver->m_feu;
				const double* fr = m_solver->m_fru;
				const double* ddx = m_solver->m_ddx;
				const double* f = dynamic_cast<const Time*>(m_solver) ? fe : fr;
				const uint32_t nu = m_solver->analysis()->assembler()->dof_unknow();
				//data
				const double t = m_tolerance;
				const double rn = math::Vector(r, nu).norm();
				const double fn = math::Vector(f, nu).norm();
				const double dxn = math::Vector(dx, nu).norm();
				const double ddxn = math::Vector(ddx, nu).norm();
				//convergence
				const bool c1 = m_type != Type::LoadFixed || rn < t;
				const bool c2 = m_type != Type::LoadRelative || rn < t * fn;
				const bool c3 = m_type != Type::StateIncrement || ddxn < t * dxn;
				//return
				return m_status = c1 && c2 && c3;
			}

			//data
			bool Convergence::status(void) const
			{
				return m_status;
			}

			Convergence::Type Convergence::type(Type type)
			{
				return m_type = type;
			}
			Convergence::Type Convergence::type(void) const
			{
				return m_type;
			}

			double Convergence::tolerance(void) const
			{
				return m_tolerance;
			}
			double Convergence::tolerance(double tolerance)
			{
				return m_tolerance = tolerance;
			}

			//name
			const char* Convergence::type_name(Type type)
			{
				const char* names[] = {"Load fixed", "Load relative", "State increment"};
				return names[math::bit_index(uint32_t(type))];
			}
		}
	}
}