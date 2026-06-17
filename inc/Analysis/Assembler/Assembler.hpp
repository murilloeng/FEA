#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		class Mesh;
		namespace elements
		{
			class Element;
		}
	}
	namespace boundary
	{
		class Constraint;
		class Dependency;
	}
	namespace analysis
	{
		class Analysis;
		namespace solvers
		{
			class Solver;
		}
	}
}

typedef std::vector<uint32_t> ulist;

namespace fea
{
	namespace analysis
	{
		class Assembler
		{
		private:
			//constructors
			Assembler(void);

			//destructor
			~Assembler(void);

			//serialization
			void load_results(void);
			void save_results(void) const;

			//dof
			void map_dof(void);

			void count_dof(void);
			void count_dof(uint32_t);
			void count_dof(const ulist&);

			void add_dof(const ulist&);
			void add_dof(uint32_t, uint32_t);

			void sort_dof(void);
			void save_dof(void);
			void apply_dof(void);
			void setup_dof(void);
			void sort_dof_lists(ulist&, ulist&) const;
			void sort_dof_nodes(const ulist&, const ulist&) const;
			void sort_dof_constraints(void) const;

			//memory
			void setup(bool);
			void cleanup(void);
			void allocate(void);

		public:
			//analysis
			void setup(void);
			void check(void) const;
			void finish(void) const;

			void record(void) const;
			void update(void) const;
			void restore(void) const;

			//dof
			uint32_t dof_know(void) const;
			uint32_t dof_local(void) const;
			uint32_t dof_total(void) const;
			uint32_t dof_unknow(void) const;
			uint32_t dof_triplet(void) const;
			uint32_t dof_nonzero(void) const;

			//map
			const int32_t* rows_map(void) const;
			const int32_t* cols_map(void) const;
			const int32_t* rows_triplet(void) const;
			const int32_t* cols_triplet(void) const;

			//assemble
			void assemble(void) const;
			void assemble_forces(void) const;
			void assemble_tangents(void) const;

			void assemble_state(void) const;
			void assemble_velocity(void) const;
			void assemble_acceleration(void) const;

			void assemble_inertial_force(void) const;
			void assemble_internal_force(void) const;

			void assemble_kinetic_energy(void) const;
			void assemble_internal_energy(void) const;

			void assemble_dead_force(void) const;
			void assemble_external_force(void) const;
			void assemble_reference_force(void) const;

			void assemble_inertia(void) const;
			void assemble_damping(void) const;
			void assemble_stiffness(void) const;

			void assemble_state_increment(void) const;
			void assemble_velocity_increment(void) const;
			void assemble_acceleration_increment(void) const;

			//apply
			void apply(void) const;
			void apply_state(void) const;
			void apply_velocity(void) const;
			void apply_initials(void) const;
			void apply_supports(void) const;
			void apply_acceleration(void) const;

			//compute
			void compute(void) const;
			void compute_joints(void) const;
			void compute_elements(void) const;
			void compute_constraints(void) const;

			//increment
			void increment_state(void) const;
			void increment_velocity(void) const;
			void increment_acceleration(void) const;

			//rotation
			void adjust_moments(double*, double*, double*, double = 1) const;

		private:
			//assembly
			void assemble_number(double, uint32_t, double*, double*) const;
			void assemble_number(double, uint32_t, uint32_t, double*) const;
			void assemble_matrix(const double*, const ulist&, double*, double = 1) const;
			void assemble_vector(const double*, const ulist&, double*, double*, double = 1) const;
			void assemble_vector(const double*, const ulist&, uint32_t, double*, double = 1) const;
			void assemble_vector(const double*, uint32_t, const ulist&, double*, double = 1) const;

			void assemble_dead_force_nodes(void) const;
			void assemble_dead_force_elements(void) const;

			void assemble_inertial_force_joints(void) const;
			void assemble_inertial_force_elements(void) const;
			void assemble_inertial_force_constraints(void) const;

			void assemble_reference_force_nodes(void) const;
			void assemble_reference_force_elements(void) const;

			void assemble_external_force_nodes(double) const;
			void assemble_external_force_elements(double) const;

			void assemble_internal_force_joints(void) const;
			void assemble_internal_force_elements(void) const;
			void assemble_internal_force_constraints(void) const;

			void assemble_inertia_joints(void) const;
			void assemble_inertia_elements(void) const;
			void assemble_inertia_constraints(void) const;

			void assemble_damping_elements(void) const;
			void assemble_damping_constraints(void) const;

			void assemble_stiffness_joints(void) const;
			void assemble_stiffness_elements(void) const;
			void assemble_stiffness_constraints(void) const;

			//data
			mutable double m_qe;
			mutable double* m_de;
			mutable double* m_ve;
			mutable double* m_ae;
			mutable double* m_fe;
			mutable double* m_Ke;
			mutable double* m_Ce;
			mutable double* m_Me;

			uint32_t m_dof_know;
			uint32_t m_dof_local;
			uint32_t m_dof_total;
			uint32_t m_dof_unknow;
			uint32_t m_dof_triplet;
			uint32_t m_dof_dependent;

			int32_t* m_rows_map;
			int32_t* m_cols_map;
			int32_t* m_rows_triplet;
			int32_t* m_cols_triplet;

			solvers::Solver* m_solver;
			static Analysis* m_analysis;

			//friends
			friend class mesh::Mesh;
			friend class analysis::Analysis;
			friend class boundary::Constraint;
			friend class mesh::elements::Element;
			friend class analysis::solvers::Solver;
		};
	}
}