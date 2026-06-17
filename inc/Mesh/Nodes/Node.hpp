#pragma once

//std
#include <string>
#include <vector>
#include <cstdint>

namespace math
{
	class Vec3;
}
namespace fea
{
	namespace mesh
	{
		class Mesh;
		namespace cells
		{
			class Cell;
			class Line;
			class Plane;
		}
		namespace nodes
		{
			enum class dof : uint32_t;
		}
		namespace elements
		{
			class Element;
		}
		namespace joints
		{
			class Joint;
		}
	}
	namespace boundary
	{
		class Initial;
		class Support;
		class Constraint;
		class Dependency;
		namespace loads
		{
			class Load;
			class Node;
		}
	}
	namespace analysis
	{
		class Analysis;
		class Assembler;
		namespace solvers
		{
			class Drift;
			class Solver;
		}
	}
}

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node
			{
			private:
				//constructors
				Node(void);

				//destructor
				~Node(void);

				//serialization
				void load(FILE*);
				void save(FILE*) const;

				void load_results(void);
				void save_results(void) const;

			public:
				//data
				static Mesh* mesh(void);

				double coordinate(uint32_t) const;
				const double* coordinates(void) const;
				const double* coordinates(const double*);
				const double* coordinates(double, uint32_t);
				const double* coordinates(double, double, double);

				//index
				uint32_t index(void) const;

				//kinematics
				double* position(double*) const;
				const double* quaternion(bool = true) const;
				const double* rotation(uint32_t = 0, bool = true) const;
				const double* translation(uint32_t = 0, bool = true) const;

				//transform
				Node& move(const math::Vec3&, bool = false);
				Node& scale(const math::Vec3&, double, bool = false);
				Node& rotate(const math::Vec3&, const math::Vec3&, bool = false);
				Node& mirror(const math::Vec3&, const math::Vec3&, bool = false);

				//state
				double& state(dof);
				double& velocity(dof);
				double& acceleration(dof);

				const double state(dof) const;
				const double velocity(dof) const;
				const double acceleration(dof) const;

				//results
				double state_data(dof, uint32_t) const;
				double velocity_data(dof, uint32_t) const;
				double acceleration_data(dof, uint32_t) const;

				//dof
				static const char* dof_name(dof);
				static const char* load_name(dof);

				uint32_t dof_set(void) const;
				uint32_t dof_size(void) const;
				uint32_t dof_index(dof) const;
				uint32_t dof_index(uint32_t) const;

			private:
				//analysis
				void apply_dof(dof);
				void apply_dof(uint32_t);
				void setup_dof(uint32_t&);

				void check(void);
				void setup(void);
				void record(void);
				void update(void);
				void restore(void);

				void setup_memory(void);
				void setup_rotation(void);

				void update_rotation(void);

				//apply
				void apply_state(void);
				void apply_velocity(void);
				void apply_acceleration(void);

				//increment
				void increment_state(void);
				void increment_velocity(void);
				void increment_acceleration(void);

				//data
				uint32_t m_index;
				double* m_quat_old;
				double* m_quat_new;
				double* m_state_old;
				double* m_state_new;
				double* m_state_data;
				double* m_velocity_old;
				double* m_velocity_new;
				double* m_velocity_data;
				double* m_acceleration_old;
				double* m_acceleration_new;
				double* m_acceleration_data;

				uint32_t m_dof_set;
				double m_coordinates[3];
				std::vector<uint32_t> m_dof_index;

				static Mesh* m_mesh;

				//friends
				friend class mesh::Mesh;
				friend class mesh::cells::Cell;
				friend class mesh::cells::Line;
				friend class mesh::cells::Plane;
				friend class mesh::joints::Joint;
				friend class mesh::elements::Element;

				friend class boundary::Initial;
				friend class boundary::Support;
				friend class boundary::Constraint;
				friend class boundary::Dependency;
				friend class boundary::loads::Load;
				friend class boundary::loads::Node;

				friend class analysis::Analysis;
				friend class analysis::Assembler;
				friend class analysis::solvers::Drift;
				friend class analysis::solvers::Solver;
			};
		}
	}
}