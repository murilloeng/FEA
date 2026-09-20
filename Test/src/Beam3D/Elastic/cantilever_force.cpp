//std
#include <cmath>

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//Math
#include "Math/inc/Validation/Validator.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam3D.hpp"

//data
static const uint32_t ne = 10;
static const double b = 1.00e-01;
static const double h = 1.00e-01;
static const double L = 1.00e+00;
static const double v = 3.00e-01;
static const double E = 2.10e+11;

//reference: doi.org/10.1002/nme.1620170113

void test::beam3D::elastic::cantilever_force(void)
{
	//data
	fea::Model model;
	sections::Rectangle section;
	materials::Uniaxial material;
	math::validation::Validator validator;
	//types
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::analysis::Type solver;
	//points
	model.geometry()->create_point(0, 0, 0);
	model.geometry()->create_point(L, 0, 0);
	//curves
	model.geometry()->create_line(0, 1);
	model.geometry()->curve(0)->structured(ne);
	model.geometry()->curve(0)->element_type(fea::mesh::elements::Type::Beam3D);
	//generate
	model.geometry()->generate_mesh();
	//elements
	for(fea::mesh::elements::Element* element : model.mesh()->elements())
	{
		((fea::mesh::elements::Beam3D*) element)->section(&section);
		((fea::mesh::elements::Beam3D*) element)->material(&material);
		((fea::mesh::elements::Beam3D*) element)->major_axis(0, 1, 0);
	}
	fea::mesh::elements::Mechanic::formulation(fea::mesh::elements::Mechanic::Formulation::Corotational);
	//supports
	model.boundary()->create_support(0, dof::Rotation_3);
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	//loads
	section.width(b);
	section.height(h);
	section.compute();
	material.poisson_ratio(v);
	material.elastic_modulus(E);
	const double I = section.inertia(0);
	model.boundary()->create_load_combination(0, false, 1);
	model.boundary()->create_load_case(1, dof::Translation_2, -E * I /  L /  L);
	//setup
	model.analysis()->type(solver::StaticNonlinear);
	model.analysis()->solver_static_nonlinear()->silent(false);
	model.analysis()->solver_static_nonlinear()->step_max(400);
	model.analysis()->solver_static_nonlinear()->load_combination(0);
	model.analysis()->solver_static_nonlinear()->watch_dof().node(1);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Rotation_3);
	model.analysis()->solver_static_nonlinear()->stop_criteria().load_max(1.00e+01);
	model.analysis()->solver_static_nonlinear()->stop_criteria().add_type(math::solvers::StopCriteria::Type::LoadLimitMaximum);
	//solve
	model.solve();
	//save
	model.save_results("Test/data/Beam 3D/Elastic/Cantilever Force");
	model.analysis()->solver_static_nonlinear()->save("Test/data/Beam 3D/Elastic/Cantilever Force/data.txt", {
		{1, dof::Translation_1}, {1, dof::Translation_2}
	});
	//validator
	validator.create_item();
	validator.create_item();
	validator.item(0)->tolerance(3.50e-02);
	validator.item(1)->tolerance(3.50e-02);
	validator.item(0)->load_numeric("Test/data/Beam 3D/Elastic/Cantilever Force/data.txt", 2, 0);
	validator.item(1)->load_numeric("Test/data/Beam 3D/Elastic/Cantilever Force/data.txt", 2, 1);
	validator.item(0)->load_reference("Test/data/Beam 3D/Elastic/Cantilever Force/reference.dat", 0, 2);
	validator.item(1)->load_reference("Test/data/Beam 3D/Elastic/Cantilever Force/reference.dat", 0, 1);
	//validate
	validator.validate();
	//draw
	fea::draw::Engine(&model).start();
}