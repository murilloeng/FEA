//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

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

	//analysis
	void Model::check(void)
	{
		m_mesh->check();
		m_boundary->check();
		m_analysis->check();
	}
	void Model::setup(void)
	{
		//dof
		m_analysis->m_assembler->dof_sort();
		//setup
		m_mesh->setup();
		m_boundary->setup();
		m_analysis->setup();
	}
	void Model::solve(void)
	{
		check();
		setup();
		m_analysis->m_solver->solve();
	}
	void Model::compute(void)
	{
		m_mesh->compute();
	}
}