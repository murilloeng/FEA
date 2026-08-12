//std
#include <cmath>

//Math
#include "Math/inc/Miscellaneous/util.hpp"

//Sections
#include "Sections/inc/Rectangle.hpp"

//Materials
#include "Materials/inc/Mechanic/Uniaxial.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

#include "FEA/inc/Geometry/Arc.hpp"
#include "FEA/inc/Geometry/Line.hpp"
#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Test
#include "FEA/Test/inc/Beam2D.hpp"

//data
static const uint32_t na = 5;
static const uint32_t nl = 5;
static const uint32_t nw = 1;
static const uint32_t nh = 40;
static const double a = 2.00e-04;
static const double P = 1.00e+00;
static const double t = 5.10e-04;
static const double v = 3.00e-01;
static const double E = 1.57e+09;
static const double Ep = 3.08e+06;
static const double ey = 2.85e-02;
static const double L1 = 4.00e-03;
static const double L2 = 1.20e-02;
static const double R0 = 4.00e-04;

static const double sy = E * ey;
static const double K = E * Ep / (E - Ep);

//reference: doi.org/10.48550/arXiv.2412.06022

void test::beam2D::inelastic::morpho_plastic_unit(void)
{
	//data
	fea::Model model;
	//points
	model.geometry()->create_point(1, 0, 0);
	//generate
	model.geometry()->generate_mesh();
	//draw
	fea::draw::Engine(&model).start();
}