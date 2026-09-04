#pragma once

//Math
#include "Math/inc/Linear/Vector.hpp"

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
				uint32_t stress_set(void) const override;
				uint32_t dof_set(uint32_t) const override;

				//tangents
				void inertia_CR(double*) const;
				void inertia_TL(double*) const;
				void inertia(double*) const override;

				void damping_CR(double*) const;
				void damping_TL(double*) const;
				void damping(double*) const override;

				void stiffness_CR(double*) const;
				void stiffness_TL(double*) const;
				void stiffness(double*) const override;

				//forces
				void internal_force_CR(double*) const;
				void internal_force_TL(double*) const;
				void internal_force(double*) const override;

				//analysis
				void update(void) override;
				void restore(void) override;
				void compute(void) override;

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

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}