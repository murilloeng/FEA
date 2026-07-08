#pragma once

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
			public:
				//constructor
				Truss(void);

				//destructor
				~Truss(void);

				//data
				static StrainMeasure strain_measure(void);
				static StrainMeasure strain_measure(StrainMeasure);

			protected:
				//analysis
				void setup(void) override;
				void compute(void) override;

				//data
				double m_f, m_K;
				double m_Lr, m_Ln, m_sr;
				double m_em, m_eg, m_eh;
				static StrainMeasure m_strain_measure;
			};
		}
	}
}