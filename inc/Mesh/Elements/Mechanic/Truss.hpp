#pragma once

//Materials
#include "Materials/inc/Mechanic/Point.hpp"

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Frame.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/StrainMeasure.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Truss : public Frame
			{
			protected:
				//constructor
				Truss(void);

				//destructor
				~Truss(void);

				//save
				void save(FILE*) const override;

			public:
				//data
				double residual_stress(double);
				double residual_stress(void) const;

				static StrainMeasure strain_measure(void);
				static StrainMeasure strain_measure(StrainMeasure);

			protected:
				//analysis
				void check(void) override;
				void setup(void) override;
				void compute(void) override;

				//data
				double m_f, m_K;
				double m_Lr, m_Ln, m_sr;
				double m_em, m_eg, m_eh;
				materials::Point m_point;
				static StrainMeasure m_strain_measure;
			};
		}
	}
}