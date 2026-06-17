//std
#include <stdexcept>
#include <functional>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/Load.hpp"
#include "FEA/inc/Boundary/Loads/Types.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Loads/Nodes/Node.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Element.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Mechanic.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Line/LineForce.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Line/LineMoment.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Plane/PlaneForce.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Plane/PlaneMoment.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Volume/VolumeForce.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructor
			Element::Element(LoadCase* load_case) : Load(load_case)
			{
				return;
			}

			//destructor
			Element::~Element(void)
			{
				return;
			}

			//serialization
			void Element::load(FILE* file)
			{
				Load::load(file);
				if(fscanf(file, "%d", &m_element) != 1)
				{
					throw std::runtime_error("Element load loading error!");
				}
			}
			void Element::save(FILE* file) const
			{
				Load::save(file);
				if(fprintf(file, "%d ", m_element) < 0)
				{
					throw std::runtime_error("Element load saving error!");
				}
			}

			//create
			void Element::create(Element*& load, Type type, LoadCase* load_case)
			{
				//data
				std::function<void(void)> factory[] = {
					[&load, load_case](void){ load = new LineForce(load_case); },
					[&load, load_case](void){ load = new LineMoment(load_case); },
					[&load, load_case](void){ load = new PlaneForce(load_case); },
					[&load, load_case](void){ load = new PlaneMoment(load_case); },
					[&load, load_case](void){ load = new VolumeForce(load_case); }
				};
				//create
				delete load;
				factory[math::bit_index(uint32_t(type))]();
			}

			//name
			const char* Element::type_name(Type type)
			{
				const char* names[] = {
					"Line Force", "Line Moment", "Plane Force", "Plane Moment", "Volume Force"
				};
				return names[math::bit_index(uint32_t(type))];
			}
			const char* Element::type_name(void) const
			{
				return type_name(type());
			}

			//data
			mesh::elements::Element* Element::element(void) const
			{
				return m_load_case->boundary()->model()->mesh()->element(m_element);
			}
			mesh::elements::Element* Element::element(uint32_t element)
			{
				return m_load_case->boundary()->model()->mesh()->element(m_element = element);
			}

			//index
			uint32_t Element::index(void) const
			{
				return m_index;
			}
			uint32_t Element::index_element(void) const
			{
				return m_element;
			}

			//analysis
			void Element::prepare(void)
			{
				return;
			}
			bool Element::check(void) const
			{
				if(m_element >= m_load_case->boundary()->model()->mesh()->elements().size())
				{
					printf("Element load %d has an out of range element index\n", m_index);
					return false;
				}
				return true;
			}
		}
	}
}