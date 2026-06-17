#pragma once

namespace fea
{
	class Model;
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

		private:
			//friends
			friend class fea::Model;

			//data
			static Model* m_model;
		};
	}
}