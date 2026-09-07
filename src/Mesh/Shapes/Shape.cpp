//std
#include <stdexcept>
#include <functional>

//FEA
#include "FEA/inc/Mesh/Shapes/Shape.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Tri3.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Tri6.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Tri9.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Tri10.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Tri12.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Tri15.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Quad4.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Quad8.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Quad9.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line2.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line3.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line4.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line5.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line6.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Tetra4.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Brick8.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Tetra10.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Brick20.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Brick27.hpp"

//Math
#include "Math/inc/Linear/Matrix.hpp"
#include "Math/inc/Miscellaneous/util.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Shape::Shape(uint32_t order) : m_quadrature{order}
			{
				return;
			}

			//destructor
			Shape::~Shape(void)
			{
				return;
			}

			//data
			math::quadrature::Quadrature& Shape::quadrature(void)
			{
				return m_quadrature;
			}

			//test
			void Shape::test_interpolation(void)
			{
				//data
				const uint32_t nt = 1000;
				double p[3], N[100], Ba[300], Bn[300], Be[300];
				Shape* shapes[] = {
					new Line2, new Line3, new Line4, new Line5, new Line6,
					new Tri3, new Tri6, new Tri9, new Tri10, new Tri12, new Tri15, new Quad4, new Quad8, new Quad9,
					new Tetra4, new Tetra10, new Brick8, new Brick20, new Brick27
				};
				const char* names[] = {
					"Line2", "Line3", "Line4", "Line5", "Line6",
					"Tri3", "Tri6", "Tri9", "Tri10", "Tri12", "Tri15", "Quad4", "Quad8", "Quad9",
					"Tetra4", "Tetra10", "Brick8", "Brick20", "Brick27"
				};
				//test
				srand(time(nullptr));
				for(uint32_t i = 0; i < sizeof(shapes) / sizeof(Shape*); i++)
				{
					const uint32_t nv = shapes[i]->vertices();
					const uint32_t nd = shapes[i]->dimension();
					for(uint32_t j = 0; j < nt; j++)
					{
						for(uint32_t k = 0; k < nd; k++) p[k] = math::randu(-1, +1);
						shapes[i]->function(N, p);
						shapes[i]->gradient(Ba, p);
						math::ndiff([shapes, i] (double* N, const double* p) { shapes[i]->function(N, p); }, Bn, p, nv, nd, 1e-5);
						math::Matrix(Be, nv, nd) = math::Matrix(Ba, nv, nd) - math::Matrix(Bn, nv, nd);
						if(math::Matrix(Be, nv, nd).norm() < 1e-5 * math::Matrix(Ba, nv, nd).norm())
						{
							printf("Shape: \"%s\" Test: %d Status: OK\n", names[i], j);
						}
						else
						{
							math::Matrix(Bn, nv, nd).print("Numeric:");
							math::Matrix(Ba, nv, nd).print("Analytic:");
							math::Matrix(Be, nv, nd).print("Error:", 1e-5);
							throw std::runtime_error("Error: Shape test failed!");
						}
					}
				}
				//delete
				for(const Shape* shape : shapes) delete shape;
			}
		}
	}
}