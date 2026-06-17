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

	//analysis
	void Model::check(void)
	{
		m_mesh->check();
		m_boundary->check();
		m_analysis->check();
	}
	void Model::setup(void)
	{
		m_mesh->setup();
		m_boundary->setup();
		m_analysis->setup();
	}
	void Model::record(void)
	{
		m_mesh->restore();
		m_boundary->record();
		m_analysis->record();
	}
	void Model::update(void)
	{
		m_mesh->update();
		m_boundary->update();
		m_analysis->update();
	}
	void Model::restore(void)
	{
		m_mesh->restore();
		m_boundary->restore();
		m_analysis->restore();
	}
	void Model::compute(void)
	{
		m_mesh->compute();
		m_boundary->compute();
		m_analysis->compute();
	}
}