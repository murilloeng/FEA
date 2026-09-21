//std
#include <cmath>

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//Math
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Miscellaneous/util.hpp"

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/TangentDrift.hpp"

void test::beam3D::test(void)
{
	//data
	fea::Model model;
	const double w = 1.00e-02;
	const double h = 2.00e-02;
	const double E = 2.10e+11;
	sections::Rectangle section;
	materials::Uniaxial material;
	//nodes
	model.mesh()->create_node(math::randu(), math::randu(), math::randu());
	model.mesh()->create_node(math::randu(), math::randu(), math::randu());
	//triad
	math::Vec3 s2, s3;
	const math::Vec3 z1 = model.mesh()->node(0)->position_ref();
	const math::Vec3 z2 = model.mesh()->node(1)->position_ref();
	(z2 - z1).unit().triad(s2, s3);
	//elements
	section.width(w);
	section.height(h);
	section.compute();
	material.elastic_modulus(E);
	model.mesh()->create_element(fea::mesh::elements::Type::Beam3D, {0, 1});
	((fea::mesh::elements::Beam3D*) model.mesh()->element(0))->section(&section);
	((fea::mesh::elements::Beam3D*) model.mesh()->element(0))->material(&material);
	((fea::mesh::elements::Beam3D*) model.mesh()->element(0))->major_axis(s2.data());
	//solver
	model.analysis()->solver_tangent_drift()->active(true);
	model.analysis()->solver_tangent_drift()->type(fea::analysis::TangentDrift::Type::InternalForce);
	//solve
	model.solve();
}