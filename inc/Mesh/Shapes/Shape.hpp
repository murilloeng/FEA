#pragma once

//std
#include <cstdint>

//Math
#include "Math/inc/Quadrature/Quadrature.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Element;
		}
	}
}

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Shape
			{
			protected:
				//constructor
				Shape(uint32_t);

				//destructor
				virtual ~Shape(void);

			public:
				//types
				enum class Type : uint32_t
				{
					Line2, Line3, Line4, Line5, Line6, 
					Triangle3, Triangle6, Triangle9, Triangle10, Triangle12, Triangle15, Quadrangle4, Quadrangle8, Quadrangle9,
					Tetraheadron4, Tetraheadron10, Hexahedron8, Hexahedron20, Hexahedron27
				};

				//data
				math::quadrature::Quadrature& quadrature(void);

				//geometry
				virtual uint32_t edges(void) const = 0;
				virtual uint32_t faces(void) const = 0;
				virtual uint32_t vertices(void) const = 0;
				virtual uint32_t dimension(void) const = 0;

				//shape
				virtual void function(double*, const double*) const = 0;
				virtual void gradient(double*, const double*) const = 0;

				//integration
				virtual uint32_t points(void) const = 0;
				virtual void point(double&, double*, uint32_t) const = 0;

				//geometry
				void position(double*, const elements::Element*, const double*) const;
				double jacobian(double*, const elements::Element*, const double*) const;
				double gradient(double*, const elements::Element*, const double*) const;

				//test
				static void test_interpolation(void);

			protected:
				//data
				uint32_t m_index;
				math::quadrature::Quadrature m_quadrature;
			};
		}
	}
}