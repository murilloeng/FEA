#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//type
			enum class StrainMeasure : uint32_t
			{
				Linear, Quadratic, Logarithmic
			};

			//strain
			double strain_measure(StrainMeasure, double);
			double strain_hessian(StrainMeasure, double);
			double strain_gradient(StrainMeasure, double);
		}
	}
}