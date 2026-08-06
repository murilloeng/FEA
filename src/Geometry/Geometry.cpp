//std
#include <stdexcept>

//gmsh
#include <gmsh.h>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"

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

		Curve* Geometry::create_line(uint32_t p1, uint32_t p2)
		{
			//data
			Line* line = new Line(p1, p2);
			const uint32_t nc = m_curves.size();
			//list
			line->m_index = nc;
			m_curves.push_back(line);
			//return
			return line;
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
		Point* Geometry::create_point(double x, double y, double z, double s)
		{
			//data
			const uint32_t np = m_points.size();
			Point* point = new Point(x, y, z, s);
			//append
			point->m_index = np;
			m_points.push_back(point);
			//return
			return point;
		}

		//mesh
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