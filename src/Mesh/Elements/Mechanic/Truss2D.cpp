//Math
#include "Math/inc/Linear/Vec2.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Analysis/Assembler/Request.hpp"
#include "FEA/inc/Analysis/Assembler/Compute.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"

//static
const fea::mesh::nodes::DOF du1 = fea::mesh::nodes::DOF::Translation_1;
const fea::mesh::nodes::DOF du2 = fea::mesh::nodes::DOF::Translation_2;

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Truss2D::Truss2D(void)
			{
				return;
			}

			//destructor
			Truss2D::~Truss2D(void)
			{
				return;
			}

			//data
			uint32_t Truss2D::dof_set(uint32_t) const
			{
				return 
					1 << uint32_t(nodes::DOF::Translation_1)|
					1 << uint32_t(nodes::DOF::Translation_2);
			}

			//analysis
			void Truss2D::compute(analysis::Compute& compute)
			{
				//data
				const double A = m_section->area();
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//strain
				const math::Vec2 x1 = z1 + u1;
				const math::Vec2 x2 = z2 + u2;
				const double Ln = (x2 - x1).norm();
				const double em = elements::strain_measure(m_strain_measure, Ln / m_Lr);
				const double eh = elements::strain_hessian(m_strain_measure, Ln / m_Lr);
				const double eg = elements::strain_gradient(m_strain_measure, Ln / m_Lr);
				//material
				double sn, Cn;
				const double E = m_material->elastic_modulus();
				((materials::Uniaxial*) m_material)->return_mapping(sn, Cn, em, m_sr, m_point);
				//compute
				const double fn = sn * A * eg;
				const double Kn = (Cn * eg * eg + sn * eh) * A / m_Lr;
				//internal energy
				if(compute.m_request->m_U)
				{
					compute.m_U = sn * sn / E / 2 * A * m_Lr;
				}
				//internal force
				if(compute.m_request->m_f)
				{
					compute.m_f[0] = -fn * (x2[0] - x1[0]) / Ln;
					compute.m_f[1] = -fn * (x2[1] - x1[1]) / Ln;
					compute.m_f[2] = +fn * (x2[0] - x1[0]) / Ln;
					compute.m_f[3] = +fn * (x2[1] - x1[1]) / Ln;
				}
				//stiffness
				if(compute.m_request->m_K)
				{
					const math::Vec2 t1 = (x2 - x1) / Ln;
					compute.m_K[0 + 4 * 0] = compute.m_K[2 + 4 * 2] = +(Kn - fn / Ln) * t1[0] * t1[0] + fn / Ln;
					compute.m_K[1 + 4 * 1] = compute.m_K[3 + 4 * 3] = +(Kn - fn / Ln) * t1[1] * t1[1] + fn / Ln;
					compute.m_K[2 + 4 * 0] = compute.m_K[0 + 4 * 2] = -(Kn - fn / Ln) * t1[0] * t1[0] - fn / Ln;
					compute.m_K[3 + 4 * 1] = compute.m_K[1 + 4 * 3] = -(Kn - fn / Ln) * t1[1] * t1[1] - fn / Ln;
					compute.m_K[1 + 4 * 0] = compute.m_K[0 + 4 * 1] = compute.m_K[3 + 4 * 2] = compute.m_K[2 + 4 * 3] = +(Kn - fn / Ln) * t1[0] * t1[1];
					compute.m_K[3 + 4 * 0] = compute.m_K[0 + 4 * 3] = compute.m_K[2 + 4 * 1] = compute.m_K[1 + 4 * 2] = -(Kn - fn / Ln) * t1[0] * t1[1];
				}
			}
		}
	}
}