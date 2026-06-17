#pragma once

//Sections
#include "Sections/inc/Section.hpp"

//Materials
#include "Materials/inc/Mechanic/Point.hpp"

//FEA
#include "FEA/inc/Mesh/Elements/Frame.hpp"
#include "FEA/inc/Mesh/Elements/StrainMeasure.hpp"

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
				virtual ~Truss(void);

				//data
				static StrainMeasure strain_measure(void);
				static StrainMeasure strain_measure(StrainMeasure);

			protected:
				//analysis
				void compute(void) override;

				//strains
				static double strain_measure(double);
				static double strain_hessian(double);
				static double strain_gradient(double);

				//data
				double m_f, m_K;
				materials::Point m_material_point;
				static StrainMeasure m_strain_measure;
			};
		}
	}
}