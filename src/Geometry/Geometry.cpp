//std
#include <stdexcept>

//gmsh
#include <gmsh.h>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"

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
			generate_mesh_elements_curves();
			generate_mesh_elements_surfaces();
		}

		void Geometry::generate_mesh_elements_curves(void) const
		{
			//data
			std::vector<int32_t> ec;
			std::vector<uint32_t> nodes;
			mesh::elements::Element* element;
			std::vector<std::vector<std::size_t>> et, en;
			uint32_t ne = m_model->mesh()->elements().size();
			//curves
			for(Curve* curve : m_curves)
			{
				gmsh::model::mesh::getElements(ec, et, en, 1, curve->m_index + 1);
				for(uint32_t j = 0; j < ec.size(); j++)
				{
					if(gmsh_cell(ec[j]) & (uint32_t) cell->type())
					{
						nodes.resize(cell->vertices());
						for(uint32_t k = 0; k < et[j].size(); k++)
						{
							for(uint32_t a = 0; a < nodes.size(); a++)
							{
								nodes[a] = en[j][k * nodes.size() + a] - 1;
							}
							curve->m_elements.push_back(ne++);
							element = m_model->mesh()->create_element(element_type, nodes, material_index, cell_index);
							((mesh::elements::Frame*) element)->major_axis(curve->m_major_axis);
						}
					}
				}
			}
		}
		void Geometry::generate_mesh_elements_surfaces(void) const
		{
			// //data
			// std::vector<int32_t> ec;
			// std::vector<uint32_t> nodes;
			// std::vector<std::vector<std::size_t>> et, en;
			// uint32_t ne = m_model->mesh()->elements().size();
			// //surfaces
			// for(surfaces::Surface* surface : m_surfaces)
			// {
			// 	if(!surface->active()) continue;
			// 	const uint32_t cell_index = surface->m_cell;
			// 	const mesh::cells::Cell* cell = surface->cell();
			// 	const uint32_t material_index = surface->m_material;
			// 	mesh::elements::Type element_type = surface->m_element;
			// 	gmsh::model::mesh::getElements(ec, et, en, 2, surface->m_index + 1);
			// 	for(uint32_t j = 0; j < ec.size(); j++)
			// 	{
			// 		if(gmsh_cell(ec[j]) & (uint32_t) cell->type())
			// 		{
			// 			nodes.resize(cell->vertices());
			// 			for(uint32_t k = 0; k < et[j].size(); k++)
			// 			{
			// 				for(uint32_t a = 0; a < nodes.size(); a++)
			// 				{
			// 					nodes[a] = en[j][k * nodes.size() + a] - 1;
			// 				}
			// 				surface->m_elements.push_back(ne++);
			// 				m_model->mesh()->create_element(element_type, nodes, material_index, cell_index);
			// 			}
			// 		}
			// 	}
			// }
		}

		//data
		Model* Geometry::m_model = nullptr;
	}
}