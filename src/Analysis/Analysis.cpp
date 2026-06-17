//std
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Miscellaneous/util.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Extra/Silencer.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		//constructors
		Analysis::Analysis(void) : m_silent{false}, m_solved{false}, m_assembler{new Assembler}, m_solver{nullptr}
		{
			Assembler::m_analysis = this;
			solvers::Solver::m_analysis = this;
			solvers::Solver::create(m_solver, solvers::Type::StaticLinear);
		}

		//destructor
		Analysis::~Analysis(void)
		{
			delete m_solver;
			delete m_assembler;
		}

		//serialization
		void Analysis::load(FILE* file)
		{
			//solver
			math::skip_lines(file, 3);
			uint32_t silent, solved, type;
			if(fscanf(file, "%d %d %d", &silent, &solved, &type) != 3)
			{
				throw std::runtime_error("Analysis loading error!");
			}
			math::skip_lines(file, 3);
			solvers::Solver::create(m_solver, solvers::Type(type));
			//load
			m_silent = silent;
			m_solved = solved;
			m_solver->load(file);
		}
		void Analysis::save(FILE* file) const
		{
			//data
			fprintf(file, "# ANALYSIS\n\n");
			fprintf(file, "## Type\n%d %d %d\n\n", m_silent, m_solved, (uint32_t) m_solver->type());
			//solver
			fprintf(file, "## Solver\n");
			m_solver->save(file);
		}

		void Analysis::load_results(void)
		{
			m_solver->load_state();
			m_solver->load_energy();
		}
		void Analysis::save_results(void) const
		{
			m_solver->save_state();
			m_solver->save_energy();
		}

		//data
		bool Analysis::silent(void) const
		{
			return m_silent;
		}
		bool Analysis::silent(bool silent)
		{
			return m_silent = silent;
		}
		bool Analysis::solved(void) const
		{
			return m_solved;
		}
		Assembler* Analysis::assembler(void) const
		{
			return m_assembler;
		}
		solvers::Solver* Analysis::solver(void) const
		{
			return m_solver;
		}
		solvers::Solver* Analysis::solver(solvers::Type type)
		{
			if(m_solver == nullptr || m_solver->type() != type)
			{
				solvers::Solver::create(m_solver, type);
			}
			return m_solver;
		}

		models::Model* Analysis::model(void)
		{
			return m_model;
		}

		//solve
		bool Analysis::solve(bool advance)
		{
			//data
			time_t t1, t2;
			char string[200];
			const uint32_t ns = m_solver->m_step_max;
			const uint32_t wn = m_solver->m_watch_dof.index_node();
			extra::Silencer* silencer = m_silent ? new extra::Silencer : nullptr;
			const char* wd = fea::mesh::nodes::Node::dof_name(m_solver->m_watch_dof.dof());
			const char* header = "*************************   Solution process started   ************************\n";
			const char* skiper = "-------------------------------------------------------------------------------\n";
			const char* footer = "*************************   Solution process ended   **************************\n";
			//setup
			m_solved = false;
			m_model->boundary()->mesh_union();
			printf("%s", header);
			m_assembler->check();
			if(!advance) m_assembler->setup();
			//header
			printf("%s", skiper);
			printf("Analysis:\n");
			printf("\tMax Steps: \t%d\n", ns);
			printf("\tWatch dof: \t%d %s\n", wn, wd);
			if(m_solver->m_combination != UINT_MAX)
			{
				printf("\tCombination: \t%s\n", m_model->boundary()->load_combination(m_solver->m_combination)->label());
			}
			printf("\tSolver Type: \t%s\n", m_solver->type_name());
			printf("%s", skiper);
			//solve
			t1 = time(nullptr);
			m_solved = m_solver->solve();
			t2 = time(nullptr);
			if(m_solved) m_assembler->save_results();
			//footer
			m_model->boundary()->mesh_split();
			const time_t td = difftime(t2, t1);
			printf("\tDone: Solution process finished!\n\n");
			printf("Solution started at:\t%s\n", math::time_format(string, t1));
			printf("Solution finished at:\t%s\n", math::time_format(string, t2));
			printf("Solution elapsed time:\t%s\n", math::time_format(string, td, false));
			printf("%s", footer);
			//delete
			delete silencer;
			//return
			return m_solved;
		}

		//analysis
		bool Analysis::check(void) const
		{
			return m_solver->checkup();
		}
		void Analysis::setup(void) const
		{
			m_solver->setup();
			m_assembler->m_solver = m_solver;
			m_solver->m_assembler = m_assembler;
		}
		void Analysis::update(void) const
		{
			m_solver->update();
		}
		void Analysis::restore(void) const
		{
			m_solver->restore();
		}
		void Analysis::apply_dof(void) const
		{
			m_solver->watch_dof().node()->apply_dof(m_solver->watch_dof().dof());
		}

		//static data
		models::Model* Analysis::m_model = nullptr;
	}
}