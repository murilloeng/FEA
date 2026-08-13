//std
#include <ranges>
#include <stdexcept>

//gmsh
#include <gmsh.h>

//Math
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Miscellaneous/BoundingBox.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"

#include "FEA/inc/Geometry/Arc.hpp"
#include "FEA/inc/Geometry/Line.hpp"
#include "FEA/inc/Geometry/Point.hpp"
#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Surface.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Geometry::Geometry(void) : m_order{1}, m_recombine{false}, m_incomplete{false}
		{
			Point::m_geometry = this;
			Curve::m_geometry = this;
			Surface::m_geometry = this;
		}

		//destructor
		Geometry::~Geometry(void)
		{
			for(Point* point : m_points) delete point;
			for(Curve* curve : m_curves) delete curve;
			for(Surface* surface : m_surfaces) delete surface;
		}

		//serialization
		void Geometry::save(FILE* file) const
		{
			//header
			fprintf(file, "# GEOMETRY\n\n");
			//mesh
			fprintf(file, "## Mesh\n%d %d %d\n", m_order, m_recombine, m_incomplete);
			//points
			fprintf(file, "\n");
			fprintf(file, "## Points: %zd\n", m_points.size());
			for(const Point* point : m_points)
			{
				point->save(file);
				fprintf(file, "\n");
			}
			//curves
			fprintf(file, "\n");
			fprintf(file, "## Curves: %zd\n", m_curves.size());
			for(const Curve* curve : m_curves)
			{
				curve->save(file);
				fprintf(file, "\n");
			}
			//surfaces
			fprintf(file, "\n");
			fprintf(file, "## Surfaces: %zd\n", m_surfaces.size());
			for(const Surface* surface : m_surfaces)
			{
				surface->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}

		//data
		Model* Geometry::model(void)
		{
			return m_model;
		}

		uint32_t Geometry::order(void) const
		{
			return m_order;
		}
		uint32_t Geometry::order(uint32_t order)
		{
			return m_order = order;
		}

		bool Geometry::recombine(void) const
		{
			return m_recombine;
		}
		bool Geometry::recombine(bool recombine)
		{
			return m_recombine = recombine;
		}

		bool Geometry::incomplete(void) const
		{
			return m_incomplete;
		}
		bool Geometry::incomplete(bool incomplete)
		{
			return m_incomplete = incomplete;
		}

		Point* Geometry::point(uint32_t index) const
		{
			if(index >= m_points.size())
			{
				throw std::runtime_error("Error: Geometry point index is out of range!");
			}
			return m_points[index];
		}
		Curve* Geometry::curve(uint32_t index) const
		{
			if(index >= m_curves.size())
			{
				throw std::runtime_error("Error: Geometry curve index is out of range!");
			}
			return m_curves[index];
		}
		Surface* Geometry::surface(uint32_t index) const
		{
			if(index >= m_surfaces.size())
			{
				throw std::runtime_error("Error: Geometry surface index is out of range!");
			}
			return m_surfaces[index];
		}

		const std::vector<Point*>& Geometry::points(void) const
		{
			return m_points;
		}
		const std::vector<Curve*>& Geometry::curves(void) const
		{
			return m_curves;
		}
		const std::vector<Surface*>& Geometry::surfaces(void) const
		{
			return m_surfaces;
		}

		//create
		Surface* Geometry::create_surface(void)
		{
			//data
			Surface* surface = new Surface();
			const uint32_t ns = m_surfaces.size();
			//list
			surface->m_index = ns;
			m_surfaces.push_back(surface);
			//return
			return surface;
		}

		Curve* Geometry::append_curve(Curve* curve)
		{
			//data
			const uint32_t nc = m_curves.size();
			//append
			curve->m_index = nc;
			m_curves.push_back(curve);
			//return
			return curve;
		}
		Curve* Geometry::create_line(uint32_t p1, uint32_t p2)
		{
			//data
			Line* line = new Line(p1, p2);
			const uint32_t nc = m_curves.size();
			//append
			line->m_index = nc;
			m_curves.push_back(line);
			//return
			return line;
		}
		Curve* Geometry::create_arc(uint32_t p1, uint32_t p2, uint32_t p3)
		{
			//data
			Arc* arc = new Arc(p1, p2, p3);
			const uint32_t nc = m_curves.size();
			//append
			arc->m_index = nc;
			m_curves.push_back(arc);
			//return
			return arc;
		}

		Point* Geometry::create_point(const double* p, double s)
		{
			//data
			Point* point = new Point(p, s);
			const uint32_t np = m_points.size();
			//append
			point->m_index = np;
			m_points.push_back(point);
			//return
			return point;
		}
		Point* Geometry::create_point(double x1, double x2, double x3, double s)
		{
			//data
			const uint32_t np = m_points.size();
			Point* point = new Point(x1, x2, x3, s);
			//append
			point->m_index = np;
			m_points.push_back(point);
			//return
			return point;
		}

		//remove
		void Geometry::remove_point(uint32_t index)
		{
			//remove curves
			for(Curve* curve : m_curves | std::views::reverse)
			{
				if(std::find(curve->m_points.begin(), curve->m_points.end(), index) != curve->m_points.end())
				{
					remove_curve(curve->m_index);
				}
			}
			//update curves
			for(Curve* curve : m_curves)
			{
				for(uint32_t& point : curve->m_points)
				{
					if(point > index) point--;
				}
			}
			//indexes
			for(uint32_t i = index + 1; i < m_points.size(); i++)
			{
				m_points[i]->m_index--;
			}
			//remove point
			delete m_points[index];
			m_points.erase(m_points.begin() + index);
		}
		void Geometry::remove_curve(uint32_t index)
		{
			//remove surfaces
			for(Surface* surface : m_surfaces | std::views::reverse)
			{
				bool test = false;
				for(const Loop& loop : surface->m_loops)
				{
					for(const Item& item : loop.m_items)
					{
						test = test || item.m_index == index;
					}
				}
				if(test)
				{
					remove_surface(surface->m_index);
				}
			}
			//update surfaces
			for(Surface* surface : m_surfaces)
			{
				for(Loop& loop : surface->m_loops)
				{
					for(Item& item : loop.m_items)
					{
						if(item.m_index > index) item.m_index--;
					}
				}
			}
			//indexes
			for(uint32_t i = index + 1; i < m_curves.size(); i++)
			{
				m_curves[i]->m_index--;
			}
			//remove curve
			delete m_curves[index];
			m_curves.erase(m_curves.begin() + index);
		}
		void Geometry::remove_surface(uint32_t index)
		{
			//indexes
			for(uint32_t i = index + 1; i < m_surfaces.size(); i++)
			{
				m_surfaces[i]->m_index--;
			}
			//remove surface
			delete m_surfaces[index];
			m_surfaces.erase(m_surfaces.begin() + index);
		}

		//transformations
		void Geometry::move_point(uint32_t index, const double* u, bool copy)
		{
			move_point(index, u[0], u[1], u[2], copy);
		}
		void Geometry::move_curve(uint32_t index, const double* u, bool copy)
		{
			move_curve(index, u[0], u[1], u[2], copy);
		}
		void Geometry::move_surface(uint32_t index, const double* u, bool copy)
		{
			move_surface(index, u[0], u[1], u[2], copy);
		}

		void Geometry::move_point(uint32_t index, double u1, double u2, double u3, bool copy)
		{
			//data
			double x[3];
			const double u[] = {u1, u2, u3};
			const double s = m_points[index]->m_size;
			const double* z = m_points[index]->m_position;
			//position
			for(uint32_t i = 0; i < 3; i++) x[i] = z[i] + u[i];
			//transform
			if(copy) create_point(x, s); else m_points[index]->position(x);
		}
		void Geometry::move_curve(uint32_t index, double u1, double u2, double u3, bool copy)
		{
			//data
			const uint32_t npt = m_points.size();
			const uint32_t npc = m_curves[index]->m_points.size();
			//move
			move_points(m_curves[index]->m_points, u1, u2, u3, copy);
			//copy
			if(copy)
			{
				Curve* curve = append_curve(m_curves[index]->clone());
				for(uint32_t i = 0; i < npc; i++) curve->m_points[i] = npt + i;
			}
		}
		void Geometry::move_surface(uint32_t index, double u1, double u2, double u3, bool copy)
		{
			return;
		}

		void Geometry::scale_point(uint32_t index, const double* x, double a, bool copy)
		{
			scale_point(index, x[0], x[1], x[2], a, copy);
		}
		void Geometry::scale_curve(uint32_t index, const double* x, double a, bool copy)
		{
			scale_curve(index, x[0], x[1], x[2], a, copy);
		}
		void Geometry::scale_surface(uint32_t index, const double* x, double a, bool copy)
		{
			scale_surface(index, x[0], x[1], x[2], a, copy);
		}

		void Geometry::scale_point(uint32_t index, const double* c, const double* a, bool copy)
		{
			scale_point(index, c[0], c[1], c[2], a[0], a[1], a[2], copy);
		}
		void Geometry::scale_curve(uint32_t index, const double* c, const double* a, bool copy)
		{
			scale_curve(index, c[0], c[1], c[2], a[0], a[1], a[2], copy);
		}
		void Geometry::scale_surface(uint32_t index, const double* c, const double* a, bool copy)
		{
			scale_surface(index, c[0], c[1], c[2], a[0], a[1], a[2], copy);
		}

		void Geometry::scale_point(uint32_t index, double c1, double c2, double c3, double a, bool copy)
		{
			//data
			double x[3];
			const double c[] = {c1, c2, c3};
			const double s = m_points[index]->m_size;
			const double* z = m_points[index]->m_position;
			//position
			for(uint32_t i = 0; i < 3; i++) x[i] = c[i] + a * (z[i] - c[i]);
			//transform
			if(copy) create_point(x, s); else m_points[index]->position(x);
		}
		void Geometry::scale_curve(uint32_t index, double c1, double c2, double c3, double a, bool copy)
		{
			//data
			const uint32_t npt = m_points.size();
			const uint32_t npc = m_curves[index]->m_points.size();
			//move
			scale_points(m_curves[index]->m_points, c1, c2, c3, a, copy);
			//copy
			if(copy)
			{
				Curve* curve = append_curve(m_curves[index]->clone());
				for(uint32_t i = 0; i < npc; i++) curve->m_points[i] = npt + i;
			}
		}
		void Geometry::scale_surface(uint32_t index, double c1, double c2, double c3, double a, bool copy)
		{
			return;
		}

		void Geometry::scale_point(uint32_t index, double c1, double c2, double c3, double a1, double a2, double a3, bool copy)
		{
			//data
			double x[3];
			const double a[] = {a1, a2, a3};
			const double c[] = {c1, c2, c3};
			const double s = m_points[index]->m_size;
			const double* z = m_points[index]->m_position;
			//position
			for(uint32_t i = 0; i < 3; i++) x[i] = c[i] + a[i] * (z[i] - c[i]);
			//transform
			if(copy) create_point(x, s); else m_points[index]->position(x);
		}
		void Geometry::scale_curve(uint32_t index, double c1, double c2, double c3, double a1, double a2, double a3, bool copy)
		{
			//data
			const uint32_t npt = m_points.size();
			const uint32_t npc = m_curves[index]->m_points.size();
			//move
			scale_points(m_curves[index]->m_points, c1, c2, c3, a1, a2, a3, copy);
			//copy
			if(copy)
			{
				Curve* curve = append_curve(m_curves[index]->clone());
				for(uint32_t i = 0; i < npc; i++) curve->m_points[i] = npt + i;
			}
		}
		void Geometry::scale_surface(uint32_t index, double c1, double c2, double c3, double a1, double a2, double a3, bool copy)
		{
			return;
		}

		void Geometry::rotate_point(uint32_t index, const double* c, const double* r, double t, bool copy)
		{
			rotate_point(index, c[0], c[1], c[2], r[0], r[1], r[2], t, copy);
		}
		void Geometry::rotate_curve(uint32_t index, const double* c, const double* r, double t, bool copy)
		{
			rotate_curve(index, c[0], c[1], c[2], r[0], r[1], r[2], t, copy);
		}
		void Geometry::rotate_surface(uint32_t index, const double* c, const double* r, double t, bool copy)
		{
			rotate_surface(index, c[0], c[1], c[2], r[0], r[1], r[2], t, copy);
		}

		void Geometry::rotate_point(uint32_t index, double c1, double c2, double c3, double r1, double r2, double r3, double t, bool copy)
		{
			//data
			const math::Vec3 c{c1, c2, c3};
			const math::Vec3 r{r1, r2, r3};
			const double s = m_points[index]->m_size;
			const math::Vec3 z = m_points[index]->m_position;
			//position
			const math::Vec3 x = c + (t * r).rotate(z - c);
			//transform
			if(copy) create_point(x.data(), s); else m_points[index]->position(x.data());
		}
		void Geometry::rotate_curve(uint32_t index, double c1, double c2, double c3, double r1, double r2, double r3, double t, bool copy)
		{
			//data
			const uint32_t npt = m_points.size();
			const uint32_t npc = m_curves[index]->m_points.size();
			//move
			rotate_points(m_curves[index]->m_points, c1, c2, c3, r1, r2, r3, t, copy);
			//copy
			if(copy)
			{
				Curve* curve = append_curve(m_curves[index]->clone());
				for(uint32_t i = 0; i < npc; i++) curve->m_points[i] = npt + i;
			}
		}
		void Geometry::rotate_surface(uint32_t index, double x1, double x2, double x3, double r1, double r2, double r3, double t, bool copy)
		{
			return;
		}

		void Geometry::move_points(std::vector<uint32_t> indexes, const double* u, bool copy)
		{
			for(uint32_t index : indexes) move_point(index, u, copy);
		}
		void Geometry::move_curves(std::vector<uint32_t> indexes, const double* u, bool copy)
		{
			for(uint32_t index : indexes) move_curve(index, u, copy);
		}
		void Geometry::move_surfaces(std::vector<uint32_t> indexes, const double* u, bool copy)
		{
			for(uint32_t index : indexes) move_surface(index, u, copy);
		}

		void Geometry::move_points(std::vector<uint32_t> indexes, double u1, double u2, double u3, bool copy)
		{
			for(uint32_t index : indexes) move_point(index, u1, u2, u3, copy);
		}
		void Geometry::move_curves(std::vector<uint32_t> indexes, double u1, double u2, double u3, bool copy)
		{
			for(uint32_t index : indexes) move_curve(index, u1, u2, u3, copy);
		}
		void Geometry::move_surfaces(std::vector<uint32_t> indexes, double u1, double u2, double u3, bool copy)
		{
			for(uint32_t index : indexes) move_surface(index, u1, u2, u3, copy);
		}

		void Geometry::scale_points(std::vector<uint32_t> indexes, const double* c, double a, bool copy)
		{
			for(uint32_t index : indexes) scale_point(index, c, a, copy);
		}
		void Geometry::scale_curves(std::vector<uint32_t> indexes, const double* c, double a, bool copy)
		{
			for(uint32_t index : indexes) scale_curve(index, c, a, copy);
		}
		void Geometry::scale_surfaces(std::vector<uint32_t> indexes, const double* c, double a, bool copy)
		{
			for(uint32_t index : indexes) scale_surface(index, c, a, copy);
		}

		void Geometry::scale_points(std::vector<uint32_t> indexes, const double* c, const double* a, bool copy)
		{
			for(uint32_t index : indexes) scale_point(index, c, a, copy);
		}
		void Geometry::scale_curves(std::vector<uint32_t> indexes, const double* c, const double* a, bool copy)
		{
			for(uint32_t index : indexes) scale_curve(index, c, a, copy);
		}
		void Geometry::scale_surfaces(std::vector<uint32_t> indexes, const double* c, const double* a, bool copy)
		{
			for(uint32_t index : indexes) scale_surface(index, c, a, copy);
		}

		void Geometry::scale_points(std::vector<uint32_t> indexes, double c1, double c2, double c3, double a, bool copy)
		{
			for(uint32_t index : indexes) scale_point(index, c1, c2, c3, a, copy);
		}
		void Geometry::scale_curves(std::vector<uint32_t> indexes, double c1, double c2, double c3, double a, bool copy)
		{
			for(uint32_t index : indexes) scale_curve(index, c1, c2, c3, a, copy);
		}
		void Geometry::scale_surfaces(std::vector<uint32_t> indexes, double c1, double c2, double c3, double a, bool copy)
		{
			for(uint32_t index : indexes) scale_surface(index, c1, c2, c3, a, copy);
		}

		void Geometry::scale_points(std::vector<uint32_t> indexes, double c1, double c2, double c3, double a1, double a2, double a3, bool copy)
		{
			for(uint32_t index : indexes) scale_point(index, c1, c2, c3, a1, a2, a3, copy);
		}
		void Geometry::scale_curves(std::vector<uint32_t> indexes, double c1, double c2, double c3, double a1, double a2, double a3, bool copy)
		{
			for(uint32_t index : indexes) scale_curve(index, c1, c2, c3, a1, a2, a3, copy);
		}
		void Geometry::scale_surfaces(std::vector<uint32_t> indexes, double c1, double c2, double c3, double a1, double a2, double a3, bool copy)
		{
			for(uint32_t index : indexes) scale_surface(index, c1, c2, c3, a1, a2, a3, copy);
		}

		void Geometry::rotate_points(std::vector<uint32_t> indexes, const double* c, const double* r, double t, bool copy)
		{
			for(uint32_t index : indexes) rotate_point(index, c, r, t, copy);
		}
		void Geometry::rotate_curves(std::vector<uint32_t> indexes, const double* c, const double* r, double t, bool copy)
		{
			for(uint32_t index : indexes) rotate_curve(index, c, r, t, copy);
		}
		void Geometry::rotate_surfaces(std::vector<uint32_t> indexes, const double* c, const double* r, double t, bool copy)
		{
			for(uint32_t index : indexes) rotate_surface(index, c, r, t, copy);
		}

		void Geometry::rotate_points(std::vector<uint32_t> indexes, double c1, double c2, double c3, double r1, double r2, double r3, double t, bool copy)
		{
			for(uint32_t index : indexes) rotate_point(index, c1, c2, c3, r1, r2, r3, t, copy);
		}
		void Geometry::rotate_curves(std::vector<uint32_t> indexes, double c1, double c2, double c3, double r1, double r2, double r3, double t, bool copy)
		{
			for(uint32_t index : indexes) rotate_curve(index, c1, c2, c3, r1, r2, r3, t, copy);
		}
		void Geometry::rotate_surfaces(std::vector<uint32_t> indexes, double c1, double c2, double c3, double r1, double r2, double r3, double t, bool copy)
		{
			for(uint32_t index : indexes) rotate_surface(index, c1, c2, c3, r1, r2, r3, t, copy);
		}

		//mesh
		void Geometry::merge(void)
		{
			//data
			std::vector<uint32_t> l1, l2;
			math::BoundingBox bounding_box;
			//bounding box
			for(const Point* point : m_points) bounding_box.insert_vertex(point->m_position);
			//query
			const double r = bounding_box.radius();
			for(uint32_t i = 0; i < m_points.size(); i++)
			{
				for(uint32_t j = 0; j < i; j++)
				{
					const math::Vec3 xi = m_points[i]->m_position;
					const math::Vec3 xj = m_points[j]->m_position;
					if((xj - xi).norm() < 1e-5 * r)
					{
						l1.push_back(i);
						l2.push_back(j);
						break;
					}
				}
			}
			//replace
			for(auto [i1, i2] : std::views::zip(l1, l2))
			{
				for(Curve* curve : m_curves)
				{
					std::replace(curve->m_points.begin(), curve->m_points.end(), i1, i2);
				}
			}
			//remove
			for(uint32_t index : l1 | std::views::reverse)
			{
				remove_point(index);
			}
		}
		void Geometry::generate_mesh(void) const
		{
			//setup
			check();
			gmsh::initialize();
			gmsh::option::setNumber("General.Verbosity", 0);
			//points
			for(const Point* point : m_points)
			{
				point->mesh();
			}
			//curves
			for(const Curve* curve : m_curves)
			{
				curve->mesh();
				if(curve->m_structured != 0) gmsh::model::geo::mesh::setTransfiniteCurve(curve->m_index + 1, curve->m_structured + 1);
			}
			//surfaces
			for(const Surface* surface : m_surfaces)
			{
				surface->mesh();
				if(surface->m_structured) gmsh::model::geo::mesh::setTransfiniteSurface(surface->m_index + 1);
			}
			//synchronize
			gmsh::model::geo::synchronize();
			//generate
			gmsh::model::mesh::generate(3);
			//post-generation
			if(m_recombine) gmsh::model::mesh::recombine();
			gmsh::option::setNumber("Mesh.SecondOrderIncomplete", m_incomplete);
			gmsh::model::mesh::setOrder(m_order);
			//recover
			generate_mesh_nodes();
			generate_mesh_elements();
			//finalize
			gmsh::finalize();
		}

		//analysis
		void Geometry::check(void) const
		{
			for(const Point* point : m_points) point->check();
			for(const Curve* curve : m_curves) curve->check();
			for(const Surface* surface : m_surfaces) surface->check();
		}

		//mesh
		void Geometry::generate_mesh_nodes(void) const
		{
			//nodes
			std::vector<double> np, nc;
			std::vector<std::size_t> nt;
			gmsh::model::mesh::getNodes(nt, nc, np);
			for(uint32_t i = 0; i < nt.size(); i++)
			{
				m_model->mesh()->create_node(nc.data() + 3 * i);
			}
			//points
			for(Point* point : m_points)
			{
				gmsh::model::mesh::getNodes(nt, nc, np, 0, point->m_index + 1);
				point->m_node = nt[0] - 1;
			}
			//curves
			for(Curve* curve : m_curves)
			{
				gmsh::model::mesh::getNodes(nt, nc, np, 1, curve->m_index + 1);
				curve->m_nodes.resize(nt.size());
				for(uint32_t i = 0; i < nt.size(); i++) curve->m_nodes[i] = nt[i] - 1;
			}
			//surfaces
			for(Surface* surface : m_surfaces)
			{
				gmsh::model::mesh::getNodes(nt, nc, np, 2, surface->m_index + 1);
				surface->m_nodes.resize(nt.size());
				for(uint32_t i = 0; i < nt.size(); i++) surface->m_nodes[i] = nt[i] - 1;
			}
		}
		void Geometry::generate_mesh_elements(void) const
		{
			//data
			std::string name;
			std::vector<double> local;
			int32_t dim, order, nn, np;
			std::vector<int32_t> types;
			std::vector<uint32_t> mesh_nodes;
			std::vector<std::vector<std::size_t>> tags, nodes;
			//curves
			for(Curve* curve : m_curves)
			{
				gmsh::model::mesh::getElements(types, tags, nodes, 1, curve->m_index + 1);
				gmsh::model::mesh::getElementProperties(types[0], name, dim, order, nn, local, np);
				mesh_nodes.resize(nn);
				for(std::size_t i = 0; i < tags[0].size(); i++)
				{
					for(int32_t j = 0; j < nn; j++) mesh_nodes[j] = nodes[0][i * nn + j] - 1;
					if(curve->m_element_type != mesh::elements::Type::Last)
					{
						m_model->mesh()->create_element(curve->m_element_type, mesh_nodes);
					}
				}
			}
			//surfaces
			for(Surface* surface : m_surfaces)
			{
				gmsh::model::mesh::getElements(types, tags, nodes, 2, surface->m_index + 1);
				gmsh::model::mesh::getElementProperties(types[0], name, dim, order, nn, local, np);
				mesh_nodes.resize(nn);
				for(std::size_t i = 0; i < tags[0].size(); i++)
				{
					for(int32_t j = 0; j < nn; j++) mesh_nodes[j] = nodes[0][i * nn + j] - 1;
					if(surface->m_element_type != mesh::elements::Type::Last)
					{
						m_model->mesh()->create_element(surface->m_element_type, mesh_nodes);
					}
				}
			}
		}

		//data
		Model* Geometry::m_model = nullptr;
	}
}