//std
#include <stdexcept>
#include <filesystem>

//Math
#include "Math/inc/Miscellaneous/util.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"

#include "FEA/inc/Results/Bounds.hpp"
#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"

namespace fea
{
	namespace models
	{
		//constructors
		Model::Model(std::string path) :
			m_saved(true), 
			m_mesh(new mesh::Mesh),
			m_results(new results::Results),
			m_boundary(new boundary::Boundary),
			m_analysis(new analysis::Analysis)
		{
			//model
			this->path(path);
			mesh::Mesh::m_model = this;
			results::Results::m_model = this;
			boundary::Boundary::m_model = this;
			analysis::Analysis::m_model = this;
		}

		// destructor
		Model::~Model(void)
		{
			//delete
			delete m_results;
			//delete
			delete m_mesh;
			delete m_boundary;
			delete m_analysis;
		}

		//serialization
		void Model::load(std::string path)
		{
			//path
			if(path.empty()) path = m_path; else m_path = path;
			//open
			FILE* file = fopen(path.c_str(), "r");
			printf("******************************   Loading model   ******************************\n");
			//file
			if(!file) throw std::runtime_error("Unable to open save file!");
			//load
			this->path(path);
			printf("\t-> Mesh\n");
			m_mesh->load(file);
			printf("\t-> Results\n");
			m_results->load(file);
			printf("\t-> Boundary\n");
			m_boundary->load(file);
			printf("\t-> Analysis\n");
			m_analysis->load(file);
			//close
			fclose(file);
			printf("\tDone!\n");
			printf("*******************************************************************************\n");
		}
		void Model::save(std::string path)
		{
			//path
			if(path.empty()) path = m_path; else m_path = path;
			std::filesystem::create_directories(std::filesystem::path(path).parent_path());
			//open
			FILE* file = fopen(path.c_str(), "w");
			printf("******************************   Saving model   *******************************\n");
			//file
			if(!file) throw std::runtime_error("Unable to create save file!");
			//save
			this->path(path);
			printf("\t-> Mesh\n");
			m_mesh->save(file);
			printf("\t-> Results\n");
			m_results->save(file);
			printf("\t-> Boundary\n");
			m_boundary->save(file);
			printf("\t-> Analysis\n");
			m_analysis->save(file);
			//close
			fclose(file);
			printf("\tDone!\n");
			printf("*******************************************************************************\n");
			//status
			m_saved = true;
		}

		void Model::load_results(void)
		{
			//check
			if(m_results->m_status) return;
			//load
			m_mesh->load_results();
			m_boundary->load_results();
			m_analysis->load_results();
			//setup
			m_results->setup();
			m_results->m_status = true;
		}
		void Model::save_results(void) const
		{
			//save
			m_mesh->save_results();
			m_boundary->save_results();
			m_analysis->save_results();
			//status
			m_results->m_status = true;
		}

		//data
		bool Model::saved(void) const
		{
			return m_saved;
		}
		void Model::mark(bool clear_selection)
		{
			m_saved = false;
			m_results->m_status = false;
			m_analysis->m_solved = false;
		}

		std::string Model::folder(void)
		{
			return std::filesystem::path(m_path).parent_path().string();
		}
		std::string Model::path(void) const
		{
			return m_path;
		}
		std::string Model::name(void) const
		{
			return m_name;
		}
		std::string Model::path(std::string path)
		{
			if(path.empty())
			{
				m_name = "Model";
				m_path = std::filesystem::current_path().string() + "/Model/Model.txt";
			}
			else if(std::filesystem::path(path).extension().empty())
			{
				m_name = "Model";
				m_path = path + "/Model.txt";
			}
			else
			{
				m_path = path;
				m_name = std::filesystem::path(path).parent_path().filename().string();
			}
			return m_path;
		}
		std::string Model::name(std::string name)
		{
			return m_name = name;
		}

		mesh::Mesh* Model::mesh(void) const
		{
			return m_mesh;
		}
		results::Results* Model::results(void) const
		{
			return m_results;
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
}