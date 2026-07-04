//FEA
#include "FEA/inc/Model.hpp"
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Analysis/Analysis.hpp"

namespace fea
{
	//constructor
	Model::Model(void) : 
		m_mesh{new mesh::Mesh}, m_boundary{new boundary::Boundary}, m_analysis{new analysis::Analysis}
	{
		mesh::Mesh::m_model = this;
		boundary::Boundary::m_model = this;
		analysis::Analysis::m_model = this;
	}
	
	//destructor
	Model::~Model(void)
	{
		delete m_mesh;
		delete m_boundary;
		delete m_analysis;
	}
}