#pragma once

//std
#include <vector>

namespace fea
{
	class Model;
	namespace boundary
	{
		class Support;
		class LoadCase;
		class LoadCombination;
	}
}

namespace fea
{
	namespace boundary
	{
		class Boundary
		{
		public:
			//constructor
			Boundary(void);

			//destructor
			~Boundary(void);

			//data
			static Model* m_model;
			std::vector<Support*> m_supports;
			std::vector<LoadCase*> m_load_cases;
			std::vector<LoadCombination*> m_load_combinations;
		};
	}
}