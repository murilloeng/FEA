//FEA
#include "FEA/inc/Model.hpp"
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Analysis/Analysis.hpp"

namespace fea
{
	//constructor
	Model::Model(void) : 
		m_mesh{new mesh::Mesh},
		m_boundary{new boundary::Boundary},
		m_analysis{new analysis::Analysis}
	{
		return;
	}

	//destructor
	Model::~Model(void)
	{
		delete m_mesh;
		delete m_boundary;
		delete m_analysis;
	}

	//data
	mesh::Mesh* Model::mesh(void) const
	{
		return m_mesh;
	}
	boundary::Boundary* Model::boundary(void) const
	{
		return m_boundary;
	}
	analysis::Analysis* Model::analysis(void) const
	{
		return m_analysis;
	}
}