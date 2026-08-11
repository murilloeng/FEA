#pragma once

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Spring : public Element
			{
			private:
				//constructor
				Spring(void);

				//destructor
				~Spring(void);

			public:
				//data
				nodes::DOF dof(nodes::DOF);
				nodes::DOF dof(void) const;

				double inertia(double);
				double inertia(void) const;

				double damping(double);
				double damping(void) const;

				double stiffness(double);
				double stiffness(void) const;

				uint32_t dof_set(uint32_t) const override;

				//tangents
				void inertia(double*) const override;
				void damping(double*) const override;
				void stiffness(double*) const override;

				//forces
				void internal_force(double*) const override;

			private:
				//analysis
				void check(void) override;
				void compute(void) override;

				//data
				nodes::DOF m_dof;
				double m_inertia;
				double m_damping;
				double m_stiffness;
			};
		}
		
	}
}