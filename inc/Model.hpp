#pragma once

namespace fea
{
	namespace mesh
	{
		class Mesh;
	}
	namespace geometry
	{
		class Geometry;
	}
	namespace boundary
	{
		class Boundary;
	}
	namespace analysis
	{
		class Solver;
		class Analysis;
	}
}

namespace fea
{
	class Model
	{
	public:
		//constructor
		Model(void);

		//destructor
		~Model(void);

		//serialization
		void save(const char*) const;
		void save_results(const char*) const;

		//data
		mesh::Mesh* mesh(void) const;
		geometry::Geometry* geometry(void) const;
		boundary::Boundary* boundary(void) const;
		analysis::Analysis* analysis(void) const;

		//solve
		void clear(void);
		void solve(void);

		protected:
		//analysis
		virtual void check(void);
		virtual void setup(void);
		virtual void update(void);
		virtual void restore(void);
		virtual void compute(void);

		//data
		mesh::Mesh* m_mesh;
		geometry::Geometry* m_geometry;
		boundary::Boundary* m_boundary;
		analysis::Analysis* m_analysis;

		//friends
		friend class fea::analysis::Solver;
	};
}