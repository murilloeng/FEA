//std
#include <cmath>

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//Math
#include "Math/inc/Linear/Vec2.hpp"
#include "Math/inc/Miscellaneous/util.hpp"

//Sections
#include "Sections/inc/CHS.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/TangentDrift.hpp"

void test::beam2D::test(void)
{
	//data
	fea::Model model;
	sections::CHS section;
	materials::Uniaxial material;
	//nodes
	model.mesh()->create_node(math::randu(), math::randu(), 0);
	model.mesh()->create_node(math::randu(), math::randu(), 0);
	//elements
	section.compute();
	material.elastic_modulus(2.00e+11);
	model.mesh()->create_element(fea::mesh::elements::Type::Beam2D, {0, 1});
	((fea::mesh::elements::Beam2D*) model.mesh()->element(0))->section(&section);
	((fea::mesh::elements::Beam2D*) model.mesh()->element(0))->material(&material);
	//solver
	model.analysis()->solver_tangent_drift()->active(true);
	model.analysis()->solver_tangent_drift()->type(fea::analysis::TangentDrift::Type::Stiffness);
	//solve
	model.solve();
}