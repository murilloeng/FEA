//std
#include <cstring>

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Fiber.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Fiber::Fiber(const sections::fibers::Fiber* fiber) : m_fiber{fiber}
			{
				return;
			}

			//destructor
			Fiber::~Fiber(void)
			{
				return;
			}

			//data
			double Fiber::area(void) const
			{
				return m_fiber->area();
			}

			const double* Fiber::warping(void) const
			{
				return m_fiber->warping();
			}
			double Fiber::warping(uint32_t index) const
			{
				return m_fiber->warping(index);
			}

			const double* Fiber::position(void) const
			{
				return m_fiber->position();
			}
			double Fiber::position(uint32_t index) const
			{
				return m_fiber->position(index);
			}

			const double* Fiber::gradient(void) const
			{
				return m_fiber->gradient();
			}
			double Fiber::gradient(uint32_t iw, uint32_t iz) const
			{
				return m_fiber->gradient(iw, iz);
			}

			materials::Point& Fiber::material_point(void)
			{
				return m_material_point;
			}
			const materials::Point& Fiber::material_point(void) const
			{
				return m_material_point;
			}
		}
	}
}