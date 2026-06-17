#pragma once

//std
#include <cstdio>
#include <string>
#include <cstdint>
#include <functional>

//FEA
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Solvers/StopCriteria.hpp"

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node;
			enum class dof : uint32_t;
		}
	}
	namespace boundary
	{
		class Initial;
		class Support;
		class Constraint;
		namespace loads
		{
			class LoadCombination;
		}
	}
	namespace analysis
	{
		class Analysis;
		class Assembler;
		namespace solvers
		{
			class WatchDof;
			enum class Type : uint32_t;
		}
	}
	namespace models
	{
		class Model;
	}
}

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			enum class state : uint32_t
			{
				x = 1 << 0,
				v = 1 << 1,
				a = 1 << 2,
				p = 1 << 3,
				t = 1 << 4
			};
			enum class force : uint32_t
			{
				r = 1 << 0,
				R = 1 << 1,
				fn = 1 << 2,
				fi = 1 << 3,
				fe = 1 << 4,
				fd = 1 << 5,
				fr = 1 << 6
			};
			enum class tangent : uint32_t
			{
				K = 1 << 0,
				C = 1 << 1,
				M = 1 << 2,
				S = 1 << 3,
				Z = 1 << 4
			};
			typedef std::function<void(void)> interface;
		}
	}
}

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Solver
			{
			protected:
				//constructors
				Solver(void);

				//destructor
				virtual ~Solver(void);

				//create
				static void create(Solver*&, Type);

				//serialization
				virtual void load(FILE*);
				virtual void save(FILE*) const;

				virtual void load_state(void);
				virtual void save_state(void) const;

				virtual void load_energy(void);
				virtual void save_energy(void) const;

			public:
				//data
				void mark(void);

				uint32_t step(void) const;
				uint32_t step_max(uint32_t);
				uint32_t step_max(void) const;

				static Analysis* analysis(void);

				StopCriteria& stop_criteria(void);
				StopCriteria& stop_criteria(uint32_t);
				const StopCriteria& stop_criteria(void) const;

				WatchDof& watch_dof(void);
				const WatchDof& watch_dof(void) const;
				WatchDof& watch_dof(uint32_t, mesh::nodes::dof);

				solvers::interface interface(void) const;
				solvers::interface interface(solvers::interface);

				uint32_t combination_index(void) const;
				boundary::loads::LoadCombination* load_combination(uint32_t);
				boundary::loads::LoadCombination* combination(void) const;

				//load
				double load_min(double);
				double load_min(void) const;

				double load_max(double);
				double load_max(void) const;

				double load_initial(double);
				double load_initial(void) const;

				double load_increment_min(double);
				double load_increment_min(void) const;

				double load_increment_max(double);
				double load_increment_max(void) const;

				double load_increment_initial(double);
				double load_increment_initial(void) const;

				//time
				double time_min(double);
				double time_min(void) const;

				double time_max(double);
				double time_max(void) const;

				double time_initial(double);
				double time_initial(void) const;

				double time_increment_min(double);
				double time_increment_min(void) const;

				double time_increment_max(double);
				double time_increment_max(void) const;

				double time_increment_initial(double);
				double time_increment_initial(void) const;

				//type
				virtual Type type(void) const = 0;
				const char* parameter(void) const;
				const char* type_name(void) const;
				static const char* type_name(Type);

				//sets
				virtual uint32_t state_set(void) const = 0;
				virtual uint32_t force_set(void) const = 0;
				virtual uint32_t tangent_set(void) const = 0;

				//results
				double state_data(uint32_t) const;
				double energy_data(uint32_t, uint32_t) const;

			protected:
				//solve
				virtual bool solve(void);
				virtual void print(void);
				virtual void record(void);
				virtual void update(void);
				virtual void restore(void);

				virtual void setup(void);
				virtual void cleanup(void);
				virtual bool checkup(void) const;

				//compute
				void compute_residue(void);
				void compute_reactions(void);

				//allocation
				void allocate_data(void);
				void allocate_state(uint32_t);
				void allocate_velocity(uint32_t);
				void allocate_acceleration(uint32_t);
				void allocate_forces(uint32_t, uint32_t);
				void allocate_tangents(uint32_t, uint32_t);
				void allocate(uint32_t, uint32_t, uint32_t);

				//linear
				void linear_delete(void);
				bool linear_decompose(const double*);

				bool linear_solve(double*, const double*, const double*);
				bool linear_substitute(double*, const double*, const double*);

				//data
				bool m_stop;
				void *m_sym, *m_num;
				WatchDof m_watch_dof;
				Assembler* m_assembler;
				static Analysis* m_analysis;
				StopCriteria m_stop_criteria;
				solvers::interface m_interface;
				uint32_t m_step, m_step_max, m_combination;

				double *m_r, *m_R;
				double *m_fnu, *m_fnk;
				double *m_fiu, *m_fik;
				double *m_feu, *m_fek;
				double *m_fru, *m_frk;
				double *m_fdu, *m_fdk;

				double *m_K, *m_C, *m_M, *m_S;
				double *m_k, *m_m, *m_z, *m_e;

				double *m_dxr, *m_dxt, *m_dvt;
				double *m_ddx, *m_ddxr, *m_ddxt;
				double *m_x_old, *m_x_new, *m_dx;
				double *m_v_old, *m_v_new, *m_dv;
				double *m_a_old, *m_a_new, *m_da;

				double *m_state_data, *m_energy_data;

				double m_t_old, m_t_new, m_t0, m_t_min, m_t_max, m_dt, m_dt0, m_dt_min, m_dt_max;
				double m_p_old, m_p_new, m_p0, m_p_min, m_p_max, m_dp, m_dp0, m_dp_min, m_dp_max, m_ddp;

				//friends
				friend class mesh::nodes::Node;

				friend class boundary::Initial;
				friend class boundary::Support;
				friend class boundary::Constraint;

				friend class analysis::Analysis;
				friend class analysis::Assembler;
				friend class analysis::solvers::StopCriteria;
			};
		}
	}
}