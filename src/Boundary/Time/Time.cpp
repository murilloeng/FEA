//std
#include <cstring>
#include <stdexcept>

//FEA
#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Time/Time.hpp"
#include "FEA/inc/Boundary/Time/Types.hpp"
#include "FEA/inc/Boundary/Time/Custom.hpp"
#include "FEA/inc/Boundary/Time/Linear.hpp"
#include "FEA/inc/Boundary/Time/SineWave.hpp"
#include "FEA/inc/Boundary/Time/Constant.hpp"
#include "FEA/inc/Boundary/Time/Quadratic.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			//constructors
			Time::Time(void) : m_label("Time")
			{
				return;
			}

			//destructor
			Time::~Time(void)
			{
				return;
			}

			//serialization
			void Time::load(FILE* file)
			{
				if(fscanf(file, "%s", m_label) != 1)
				{
					throw std::runtime_error("Time loading error!");
				}
			}
			void Time::save(FILE* file) const
			{
				fprintf(file, "%s ", m_label);
			}

			//create
			void Time::create(Time*& time, const Time* base)
			{
				create(time, base->type(), base);
			}
			void Time::create(Time*& time, time::Type type, const Time* base)
			{
				//data
				delete time;
				const std::function<void(void)> factory[] = {
					[&time, type, base](void){
						time = base ? new Custom(*(Custom*) base) : new Custom;
					},
					[&time, type, base](void){
						time = base ? new Linear(*(Linear*) base) : new Linear;
					},
					[&time, type, base](void){
						time = base ? new SineWave(*(SineWave*) base) : new SineWave;
					},
					[&time, type, base](void){
						time = base ? new Constant(*(Constant*) base) : new Constant;
					},
					[&time, type, base](void){
						time = base ? new Quadratic(*(Quadratic*) base) : new Quadratic;
					}
				};
				//create
				for(uint32_t i = 0; i < uint32_t(Type::last); i++)
				{
					if(uint32_t(type) == i)
					{
						factory[i]();
						break;
					}
				}
			}

			//data
			Boundary* Time::boundary(void)
			{
				return m_boundary;
			}

			const char* Time::label(void) const
			{
				return m_label;
			}
			const char* Time::label(const char* label)
			{
				return (const char*) strcpy(m_label, label);
			}

			//name
			const char* Time::type_name(void) const
			{
				return type_name(type());
			}
			const char* Time::type_name(time::Type type)
			{
				const char* names[] = {"Custom", "Linear", "Sine Wave", "Constant", "Quadratic"};
				return names[uint32_t(type)];
			}

			//index
			uint32_t Time::index(void) const
			{
				return m_index;
			}

			//static data
			Boundary* Time::m_boundary = nullptr;
		}
	}
}