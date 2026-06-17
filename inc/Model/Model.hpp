#pragma once

//std
#include <string>
#include <functional>

namespace fea
{
	namespace mesh
	{
		class Mesh;
	}
	namespace results
	{
		class Results;
	}
	namespace boundary
	{
		class Boundary;
	}
	namespace analysis
	{
		class Analysis;
		class Assembler;
	}
}

namespace fea
{
	namespace models
	{
		class Model
		{
		public:
			//constructors
			Model(std::string = "");

			//destructor
			~Model(void);

			//serialization
			void load(std::string = "");
			void save(std::string = "");

			void load_results(void);
			void save_results(void) const;

			//data
			void mark(bool = true);
			bool saved(void) const;

			std::string folder(void);
			std::string path(void) const;
			std::string name(void) const;
			std::string path(std::string);
			std::string name(std::string);

			mesh::Mesh* mesh(void) const;
			results::Results* results(void) const;
			boundary::Boundary* boundary(void) const;
			analysis::Analysis* analysis(void) const;

		private:
			//data
			bool m_saved;
			std::string m_path;
			std::string m_name;
			mesh::Mesh* m_mesh;
			results::Results* m_results;
			boundary::Boundary* m_boundary;
			analysis::Analysis* m_analysis;

			//friends
			friend class analysis::Analysis;
			friend class analysis::Assembler;
		};
	}
}