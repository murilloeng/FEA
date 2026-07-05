//std
#include <cmath>

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/StrainMeasure.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//strain
			double strain_measure(StrainMeasure strain_measure, double stretch)
			{
				return
					strain_measure == StrainMeasure::Linear ? stretch - 1 : 
					strain_measure == StrainMeasure::Quadratic ? (stretch * stretch - 1) / 2 : 
					strain_measure == StrainMeasure::Logarithmic ? log(stretch) : 0;
			}
			double strain_hessian(StrainMeasure strain_measure, double stretch)
			{
				return
					strain_measure == StrainMeasure::Linear ? 0 : 
					strain_measure == StrainMeasure::Quadratic ? 1 : 
					strain_measure == StrainMeasure::Logarithmic ? -1 / stretch / stretch : 0;
			}
			double strain_gradient(StrainMeasure strain_measure, double stretch)
			{
				return
					strain_measure == StrainMeasure::Linear ? 1 : 
					strain_measure == StrainMeasure::Quadratic ? stretch : 
					strain_measure == StrainMeasure::Logarithmic ? 1 / stretch: 0;
			}
		}
	}
}