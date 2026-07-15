#pragma once

namespace fea
{
	namespace mesh
	{
		class Mesh;
	}
	namespace boundary
	{
		class Boundary;
	}
	namespace analysis
	{
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
		boundary::Boundary* boundary(void) const;
		analysis::Analysis* analysis(void) const;

		//solve
		void clear(void);
		void solve(void);
		void compute(void);

	protected:
		//analysis
		virtual void check(void);
		virtual void setup(void);

		//data
		mesh::Mesh* m_mesh;
		boundary::Boundary* m_boundary;
		analysis::Analysis* m_analysis;
	};
}