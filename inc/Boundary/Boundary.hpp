#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	class Model;
	namespace boundary
	{
		class Support;
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
			static Model* model(void);

			Support* support(uint32_t) const;
			const std::vector<Support*>& supports(void) const;

		private:
			//analysis
			void check(void);
			void setup(void);
			void record(void);
			void update(void);
			void restore(void);
			void compute(void);

			//friends
			friend class fea::Model;

			//data
			static Model* m_model;
			std::vector<Support*> m_supports;
		};
	}
}