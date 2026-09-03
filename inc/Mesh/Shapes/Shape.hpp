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
				~Shape(void);

			public:
				//types
				enum class Type : uint32_t
				{
					Line2, Line3, Line4, 
					Tri3, Tri6, Tri9, Tri10, Tri12, Tri15, Quad4, Quad8, Quad9,
					Tetra4, Tetra10, Brick8, Brick20, Brick27
				};

				//data
				math::quadrature::Quadrature& quadrature(void);

				//shape
				virtual uint32_t dimension(void) const = 0;
				virtual void function(double*, const double*) const = 0;
				virtual void gradient(double*, const double*) const = 0;

				//integration
				virtual uint32_t points(void) const = 0;
				virtual void point(double&, double*, uint32_t) const = 0;

				//geometry
				void position(double*, const elements::Element*, const double*) const;
				double jacobian(double*, const elements::Element*, const double*) const;
				double gradient(double*, const elements::Element*, const double*) const;

			protected:
				//data
				uint32_t m_index;
				math::quadrature::Quadrature m_quadrature;
			};
		}
	}
}