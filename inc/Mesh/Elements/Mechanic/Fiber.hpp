#pragma once

//Sections
#include "Sections/inc/Fibers/Fiber.hpp"

//Materials
#include "Materials/inc/Mechanic/Point.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Fiber
			{
			public:
				//constructor
				Fiber(const sections::fibers::Fiber*);

				//destructor
				~Fiber(void);

				//data
				double area(void) const;

				double warping(uint32_t) const;
				const double* warping(void) const;

				double position(uint32_t) const;
				const double* position(void) const;

				const double* gradient(void) const;
				double gradient(uint32_t, uint32_t) const;

				materials::Point& material_point(void);
				const materials::Point& material_point(void) const;

			private:
				//data
				materials::Point m_material_point;
				const sections::fibers::Fiber* m_fiber;
			};
		}
	}
}