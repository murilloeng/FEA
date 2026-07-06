#pragma once

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Load
			{
			public:
				//constructor
				Load(void);

				//destructor
				virtual ~Load(void);

				//analysis
				virtual void check(void);
				virtual void setup(void);

				//data
				double m_value;
			};
		}
	}
}