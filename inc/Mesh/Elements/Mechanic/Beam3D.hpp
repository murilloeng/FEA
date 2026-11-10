#pragma once

//Math
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Linear/Vector.hpp"
#include "Math/inc/Linear/Matrix.hpp"

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Beam.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Beam3D : public Beam
			{
			public:
				//constructor
				Beam3D(void);

				//destructor
				~Beam3D(void);

				//data
				math::Vec3& major_axis(void);
				const math::Vec3& major_axis(void) const;

				uint32_t stress_set(void) const override;
				uint32_t dof_set(uint32_t) const override;

				//tangents
				void inertia_CR(double*) const;
				void inertia_TL(double*) const;

				void damping_CR(double*) const;
				void damping_TL(double*) const;

				void stiffness_CR(double*) const;
				void stiffness_TL(double*) const;

				//forces
				void internal_force_CR(double*) const;
				void internal_force_TL(double*) const;

				//energies
				double kinetic_energy_CR(void) const;
				double kinetic_energy_TL(void) const;
				double internal_energy_CR(void) const;
				double internal_energy_TL(void) const;

				//analysis
				void check(void) override;
				void setup(void) override;
				void update(void) override;
				void restore(void) override;
				void compute(analysis::Compute&) override;

				//compute
				void compute_CR(void);
				void compute_TL(void);

				//compute CR
				void compute_CR_state(void);
				void compute_CR_elastic(void);
				void compute_CR_plastic(void);
				void compute_CR_kinematic(void) const;
				void compute_CR_plastic_length(double*, double);
				void compute_CR_plastic_section(double*, double, double);

				//data
				math::Matrix m_Kl;
				math::Vector m_dl, m_fl;
				static math::Matrix m_B;

				math::Vec3 m_major_axis;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}