//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include <gmsh.h>

//Test
#include "FEA/Test/inc/Beam2D.hpp"
#include "FEA/Test/inc/Truss2D.hpp"
#include "FEA/Test/inc/Truss3D.hpp"
#include "FEA/Test/inc/Rigid2D.hpp"

int main(void)
{
	try
	{
		gmsh::initialize();
    gmsh::model::add("single_brick8");

    // 8 corner points
    gmsh::model::geo::addPoint(-1, -1, -1, 1.0, 1);
    gmsh::model::geo::addPoint(+1, -1, -1, 1.0, 2);
    gmsh::model::geo::addPoint(+1, +1, -1, 1.0, 3);
    gmsh::model::geo::addPoint(-1, +1, -1, 1.0, 4);
    gmsh::model::geo::addPoint(-1, -1, +1, 1.0, 5);
    gmsh::model::geo::addPoint(+1, -1, +1, 1.0, 6);
    gmsh::model::geo::addPoint(+1, +1, +1, 1.0, 7);
    gmsh::model::geo::addPoint(-1, +1, +1, 1.0, 8);

    // 12 edges
    gmsh::model::geo::addLine(1, 2, 1);
    gmsh::model::geo::addLine(2, 3, 2);
    gmsh::model::geo::addLine(3, 4, 3);
    gmsh::model::geo::addLine(4, 1, 4);
    gmsh::model::geo::addLine(5, 6, 5);
    gmsh::model::geo::addLine(6, 7, 6);
    gmsh::model::geo::addLine(7, 8, 7);
    gmsh::model::geo::addLine(8, 5, 8);
    gmsh::model::geo::addLine(1, 5, 9);
    gmsh::model::geo::addLine(2, 6, 10);
    gmsh::model::geo::addLine(3, 7, 11);
    gmsh::model::geo::addLine(4, 8, 12);

    // 6 curve loops (one per face), consistent line orientations
    gmsh::model::geo::addCurveLoop({1, 2, 3, 4}, 1);          // bottom, z = -1
    gmsh::model::geo::addCurveLoop({5, 6, 7, 8}, 2);          // top,    z = +1
    gmsh::model::geo::addCurveLoop({1, 10, -5, -9}, 3);       // front,  y = -1
    gmsh::model::geo::addCurveLoop({3, 12, -7, -11}, 4);      // back,   y = +1
    gmsh::model::geo::addCurveLoop({-4, 12, 8, -9}, 5);       // left,   x = -1
    gmsh::model::geo::addCurveLoop({2, 11, -6, -10}, 6);      // right,  x = +1

    // 6 plane surfaces
    gmsh::model::geo::addPlaneSurface({1}, 1);
    gmsh::model::geo::addPlaneSurface({2}, 2);
    gmsh::model::geo::addPlaneSurface({3}, 3);
    gmsh::model::geo::addPlaneSurface({4}, 4);
    gmsh::model::geo::addPlaneSurface({5}, 5);
    gmsh::model::geo::addPlaneSurface({6}, 6);

    // Surface loop + volume
    gmsh::model::geo::addSurfaceLoop({1, 2, 3, 4, 5, 6}, 1);
    gmsh::model::geo::addVolume({1}, 1);

    // --- Force each edge to have exactly 1 element (2 nodes) ---
    for (int line = 1; line <= 12; ++line) {
        gmsh::model::geo::mesh::setTransfiniteCurve(line, 2); // 2 points = 1 segment
    }

    // --- Mark each face as transfinite (structured quad) ---
    for (int surf = 1; surf <= 6; ++surf) {
        gmsh::model::geo::mesh::setTransfiniteSurface(surf);
        gmsh::model::geo::mesh::setRecombine(2, surf); // quads instead of triangles
    }

    // --- Mark the volume as transfinite (structured hex) ---
    gmsh::model::geo::mesh::setTransfiniteVolume(1);

    // Sync CAD kernel with the model before meshing
    gmsh::model::geo::synchronize();

    // Tag the volume as a physical group (optional but recommended)
    gmsh::model::addPhysicalGroup(3, {1}, 1);

	  // --- Set 2nd order mesh ---
    gmsh::option::setNumber("Mesh.ElementOrder", 2);

    // Use "incomplete" (serendipity, 20-node) or "complete" (27-node) basis
    gmsh::option::setNumber("Mesh.SecondOrderIncomplete", 0); // 1 = Brick20, 0 = Brick27

    // Generate a 3D mesh and save
    gmsh::model::mesh::generate(3);
    gmsh::write("single_brick8.msh");
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	return EXIT_SUCCESS;
}