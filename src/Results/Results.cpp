//std
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/util.hpp"

//FEA

#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Results/What.hpp"
#include "FEA/inc/Results/Bounds.hpp"
#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace results
	{
		//constructors
		Results::Results(void) : 
			m_status(false), m_steps(0), m_what(new What), m_bounds(new Bounds)
		{
			m_bounds->m_results = this;
		}

		//destructor
		Results::~Results(void)
		{
			delete m_what;
			delete m_bounds;
		}

		//serialization
		void Results::load(FILE* file)
		{
			//header
			math::skip_lines(file, 3);
			//what
			m_what->load(file);
			//steps
			math::skip_lines(file, 3);
			if(fscanf(file, "%d", &m_steps) != 1)
			{
				throw std::runtime_error("Results loading failed!");
			}
			math::skip_lines(file, 2);
		}
		void Results::save(FILE* file) const
		{
			//header
			fprintf(file, "# RESULTS\n\n");
			//what
			fprintf(file, "## What\n");
			m_what->save(file);
			fprintf(file, "\n");
			//steps
			fprintf(file, "## Steps\n%d\n\n", m_steps);
		}

		//data
		bool Results::status(void) const
		{
			return m_status;
		}
		uint32_t Results::steps(void) const
		{
			return m_steps;
		}
		uint32_t Results::dof_set(void) const
		{
			return m_dof_set;
		}
		uint32_t Results::state_set(void) const
		{
			return m_state_set;
		}

		What* Results::what(void) const
		{
			return m_what;
		}
		Bounds* Results::bounds(void) const
		{
			return m_bounds;
		}
		const models::Model* Results::model(void)
		{
			return m_model;
		}

		//setup
		void Results::setup(void)
		{
			apply_dof();
			apply_state();
			m_bounds->setup();
		}

		//apply
		void Results::apply_dof(void)
		{
			m_dof_set = 0;
			for(const fea::mesh::nodes::Node* node : m_model->mesh()->nodes())
			{
				m_dof_set |= node->dof_set();
			}
		}
		void Results::apply_state(void)
		{
			m_state_set = 0;
			for(const fea::mesh::elements::Element* element : m_model->mesh()->elements())
			{
				m_state_set |= element->states_set();
			}
		}

		//static data
		const models::Model* Results::m_model = nullptr;
	}
}