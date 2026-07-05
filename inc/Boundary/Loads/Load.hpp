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
				~Load(void);

				//data
				double m_value;
			};
		}
	}
}