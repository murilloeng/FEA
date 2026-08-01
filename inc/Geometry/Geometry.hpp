#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>

namespace fea
{
	class Model;
	namespace geometry
	{
		class Point;
		class Curve;
		class Surface;
	}
}

namespace fea
{
	namespace geometry
	{
		class Geometry
		{
		protected:
			//constructor
			Geometry(void);

			//destructor
			~Geometry(void);

			//serialization
			void save(FILE*) const;

		public:
			//data
			static Model* model(void);

			uint32_t order(uint32_t);
			uint32_t order(void) const;

			bool recombine(bool);
			bool recombine(void) const;

			bool incomplete(bool);
			bool incomplete(void) const;

			Point* point(uint32_t) const;
			Curve* curve(uint32_t) const;
			Surface* surface(uint32_t) const;

			const std::vector<Point*>& points(void) const;
			const std::vector<Curve*>& curves(void) const;
			const std::vector<Surface*>& surfaces(void) const;

			//create
			Surface* create_surface(void);

			Curve* create_line(uint32_t, uint32_t);

			Point* create_point(const double*, double = 0);
			Point* create_point(double, double, double, double = 0);

			//mesh
			void generate_mesh(void) const;

		protected:
			//mesh
			void generate_mesh_nodes(void) const;
			void generate_mesh_elements(void) const;

			void generate_mesh_elements_curves(void) const;
			void generate_mesh_elements_surfaces(void) const;

			//data
			uint32_t m_order;
			bool m_recombine;
			bool m_incomplete;
			static Model* m_model;
			std::vector<Point*> m_points;
			std::vector<Curve*> m_curves;
			std::vector<Surface*> m_surfaces;

			//friends
			friend class fea::Model;
		};
	}
}