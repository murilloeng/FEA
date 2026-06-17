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

		//data
		mesh::Mesh* mesh(void) const;
		boundary::Boundary* boundary(void) const;
		analysis::Analysis* analysis(void) const;

	private:
		//analysis
		void check(void);
		void setup(void);
		void record(void);
		void update(void);
		void restore(void);
		void compute(void);

		//data
		mesh::Mesh* m_mesh;
		boundary::Boundary* m_boundary;
		analysis::Analysis* m_analysis;
	};
}