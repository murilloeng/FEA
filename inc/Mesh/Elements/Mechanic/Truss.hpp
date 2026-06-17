#pragma once

//std
#include <cstdio>

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Frame.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			enum class StrainMeasure : uint32_t;
		}
	}
	namespace boundary
	{
		namespace loads
		{
			class LineForce;
		}
	}
}

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Truss : public Frame
			{
			protected:
				//contructors
				Truss(void);

				//destructors
				virtual ~Truss(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//types
				uint32_t loads_set(void) const override;
				uint64_t states_set(void) const override;

				//data
				bool tension(bool);
				bool tension(void) const;

				double residual_stress(double);
				double residual_stress(void) const;

				static StrainMeasure strain_measure(void);
				static StrainMeasure strain_measure(StrainMeasure);

			protected:
				//analysis
				void record(void) override;
				void compute(void) override;

				//strain
				static double strain_measure(double);
				static double strain_hessian(double);
				static double strain_gradient(double);

				//loads
				void reference_force(double*, const boundary::loads::Element*) const override;
				virtual void load_line_force(double*, const boundary::loads::LineForce*) const = 0;

				//data
				bool m_tension;
				double m_f, m_K, m_sr;
				static StrainMeasure m_strain_measure;

				//friends
				friend class Element;
			};
		}
	}
}