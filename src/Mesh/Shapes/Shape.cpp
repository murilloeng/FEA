//std
#include <stdexcept>
#include <functional>

//FEA
#include "FEA/inc/Mesh/Shapes/Shape.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line2.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line3.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line4.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line5.hpp"
#include "FEA/inc/Mesh/Shapes/1D/Line6.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Triangle3.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Triangle6.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Triangle9.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Triangle10.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Triangle12.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Triangle15.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Quadrangle4.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Quadrangle8.hpp"
#include "FEA/inc/Mesh/Shapes/2D/Quadrangle9.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Hexahedron8.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Hexahedron20.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Hexahedron27.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Tetrahedron4.hpp"
#include "FEA/inc/Mesh/Shapes/3D/Tetrahedron10.hpp"

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
					new Triangle3, new Triangle6, new Triangle9, new Triangle10, new Triangle12, new Triangle15, new Quadrangle4, new Quadrangle8, new Quadrangle9,
					new Tetrahedron4, new Tetrahedron10, new Hexahedron8, new Hexahedron20, new Hexahedron27
				};
				const char* names[] = {
					"Line2", "Line3", "Line4", "Line5", "Line6",
					"Tri3", "Tri6", "Tri9", "Tri10", "Tri12", "Tri15", "Quad4", "Quad8", "Quad9",
					"Tetrahedron4", "Tetrahedron10", "Hexahedron8", "Hexahedron20", "Hexahedron27"
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