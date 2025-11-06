#include "brep/body.h"
#include "brep/brep.fixtures.h"

/**
 * BRepFixtures provides a set of fixtures for testing or building simple breps.
 */

namespace e2 {
    namespace BRepFixtures {
        Body* createEmptyBody() {
            // A body with no cells
            Body* body = new Body;
            return body;
        }
        Body* createAcornBody(const Vec3d& position) {
            // A single point cell
            Body* body = new Body({
                Cell(position)
            });
            return body;
        }
        Body* createWireRectangle(const Vec3d& lowerLeft, const Vec3d& upperRight) {
            // A 4-sided rectangle on the z=0 plane, no interior

            // project the input positions to z=0 plane
            Vec3d ll(lowerLeft.x(), lowerLeft.y(), 0); // lower-left
            Vec3d ur(upperRight.x(), upperRight.y(), 0); // upper-right
            Vec3d lr(ur.x(), ll.y(), ll.z()); // lower-right
            Vec3d ul(ll.x(), ur.y(), ur.z()); // upper-left

            // construct the edge geometries
            Ray3d lower(ll, Vec3d(1,0,0));
            Ray3d right(lr, Vec3d(0,1,0));
            Ray3d upper(ur, Vec3d(-1,0,0));
            Ray3d left(ul, Vec3d(0,-1,0));

            // construct the cells
            std::vector<Cell> cells = {
                Cell(ll),  // 0
                Cell(lr),  // 1
                Cell(ur),  // 2
                Cell(ul),  // 3
                Cell(lower), // 4
                Cell(right), // 5
                Cell(upper), // 6
                Cell(left)   // 7
            };

            // construct the cocells
            std::vector<Cocell> cocells = {
                Cocell(4, 0, -1),  // lower edge bounded by lower-left vertex
                Cocell(4, 1, +1),  // lower edge bounded by lower-right vertex
                Cocell(5, 1, -1),  // right edge bounded by lower-right vertex
                Cocell(5, 2, +1),  // right edge bounded by upper-right vertex
                Cocell(6, 2, -1),  // upper edge bounded by upper-right vertex
                Cocell(6, 3, +1),  // upper edge bounded by upper-left vertex
                Cocell(7, 3, -1),  // left edge bounded by upper-left vertex
                Cocell(7, 0, +1)   // left edge bounded by lower-left vertex
            };

            Body* body = new Body(cells, cocells);
            return body;
        }
        Body* createSheetRectangle(const Vec3d& lowerLeft, const Vec3d& upperRight) {
            // A 4-sided rectangle on the z=0 plane, with interior
            Body* body = createWireRectangle(lowerLeft, upperRight);
            // Add a face cell for the interior
            Cell faceCell(Pla3d(Vec3d(0,0,0), Vec3d(0,0,1))); // face on z=0 plane
            size_t faceCellIndex = body->addCell(faceCell);
            // Add cocells connecting the face to its boundary edges
            body->addCocell(Cocell(faceCellIndex, 4, +1)); // face bounded by lower edge
            body->addCocell(Cocell(faceCellIndex, 5, +1)); // face bounded by right edge
            body->addCocell(Cocell(faceCellIndex, 6, +1)); // face bounded by upper edge
            body->addCocell(Cocell(faceCellIndex, 7, +1)); // face bounded by left edge
            body->updateGraph();

            return body;
        }
        
    };
};

