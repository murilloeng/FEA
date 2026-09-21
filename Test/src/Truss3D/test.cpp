//Test
#include "FEA/Test/inc/Truss3D.hpp"

//Math
#include "Math/inc/Linear/Vec3.hpp"

//Sections
#include "Sections/inc/CHS.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss3D.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/TangentDrift.hpp"

void test::truss3D::test(void)
{
	//data
	fea::Model model;
	sections::CHS section;
	materials::Uniaxial material;
	//nodes
	model.mesh()->create_node(math::Vec3().randu().data());
	model.mesh()->create_node(math::Vec3().randu().data());
	//elements
	section.compute();
	material.elastic_modulus(2.00e+11);
	model.mesh()->create_element(fea::mesh::elements::Type::Truss3D, {0, 1});
	((fea::mesh::elements::Truss3D*) model.mesh()->element(0))->section(&section);
	((fea::mesh::elements::Truss3D*) model.mesh()->element(0))->material(&material);
	//solver
	model.analysis()->solver_tangent_drift()->active(true);
	model.analysis()->solver_tangent_drift()->type(fea::analysis::TangentDrift::Type::Stiffness);
	//solve
	model.solve();
}