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

		//analysis
		void check(void);
		void setup(void);
		void solve(void);

		//data
		mesh::Mesh* m_mesh;
		boundary::Boundary* m_boundary;
		analysis::Analysis* m_analysis;
	};
}