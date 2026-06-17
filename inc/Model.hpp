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
		//data
		mesh::Mesh* m_mesh;
		boundary::Boundary* m_boundary;
		analysis::Analysis* m_analysis;
	};
}