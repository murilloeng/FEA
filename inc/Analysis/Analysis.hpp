#pragma once

namespace fea
{
	class Model;
}

namespace fea
{
	namespace analysis
	{
		class Analysis
		{
		public:
			//constructor
			Analysis(void);

			//destructor
			~Analysis(void);

			//data
			static Model* m_model;
		};
	}
}