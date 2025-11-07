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

        Body* createWireRoundRect(const Vec3d& lowerLeft, const Vec3d& upperRight, double cornerRadius) {
            // A rectangle with rounded corners on the z=0 plane, no interior

            double r = cornerRadius; // corner radius

            // project the input positions to z=0 plane
            Vec3d ll(lowerLeft.x(), lowerLeft.y(), 0); // lower-left
            Vec3d ur(upperRight.x(), upperRight.y(), 0); // upper-right
            Vec3d lr(ur.x(), ll.y(), ll.z()); // lower-right
            Vec3d ul(ll.x(), ur.y(), ur.z()); // upper-left

            // compute the endpoints of the bottom, right, top and left edges, offsetting to account for corner radius
            Vec3d b1(ll.x() + r, ll.y(), ll.z()); // lower-left horizontal start
            Vec3d b2(lr.x() - r, lr.y(), lr.z()); // lower-right horizontal end
            Vec3d r1(lr.x(), lr.y() + r, lr.z()); // lower-right vertical start
            Vec3d r2(ur.x(), ur.y() - r, ur.z()); // upper-right vertical end
            Vec3d t1(ur.x() - r, ur.y(), ur.z()); // upper-right horizontal start
            Vec3d t2(ul.x() + r, ul.y(), ul.z()); // upper-left horizontal end
            Vec3d l1(ul.x(), ul.y() - r, ul.z()); // upper-left vertical start
            Vec3d l2(ll.x(), ll.y() + r, ll.z()); // lower-left vertical end

            // compute the centers of the corner arcs
            Vec3d c_bl(b1.x(), l2.y(), ll.z()); // bottom-left corner center
            Vec3d c_br(b2.x(), r1.y(), lr.z()); // bottom-right corner center
            Vec3d c_tr(t1.x(), r2.y(), ur.z()); // top-right corner center
            Vec3d c_tl(t2.x(), l1.y(), ul.z()); // top-left corner center

            // construct the edge geometries
            Ray3d bottom(ll, Vec3d(1,0,0));
            Ray3d right(lr, Vec3d(0,1,0));
            Ray3d top(ur, Vec3d(-1,0,0));
            Ray3d left(ul, Vec3d(0,-1,0));

            Cir3d corner_bl(c_bl, r, Vec3d(0,0,1), Vec3d(1,0,0)); // bottom-left corner arc
            Cir3d corner_br(c_br, r, Vec3d(0,0,1), Vec3d(1,1,0)); // bottom-right corner arc
            Cir3d corner_tr(c_tr, r, Vec3d(0,0,1), Vec3d(1,0,0)); // top-right corner arc
            Cir3d corner_tl(c_tl, r, Vec3d(0,0,1), Vec3d(1,0,0)); // top-left corner arc

            // construct the cells
            std::vector<Cell> cells = {
                Cell(b1),             // 0
                Cell(b2),             // 1    
                Cell(r1),             // 2
                Cell(r2),             // 3
                Cell(t1),             // 4
                Cell(t2),             // 5
                Cell(l1),             // 6
                Cell(l2),             // 7
                Cell(bottom),         // 8
                Cell(right),          // 9
                Cell(top),            // 10
                Cell(left),           // 11
                Cell(corner_bl),      // 12
                Cell(corner_br),      // 13
                Cell(corner_tr),      // 14
                Cell(corner_tl),      // 15
            };

            // construct the cocells
            std::vector<Cocell> cocells = {
                Cocell(8, 0, -1),  
                Cocell(8, 1, +1),  
                Cocell(9, 2, -1),  
                Cocell(9, 3, +1),  
                Cocell(10, 4, -1),  
                Cocell(10, 5, +1),  
                Cocell(11, 6, -1),  
                Cocell(11, 7, +1),  
                Cocell(12, 7, -1),  
                Cocell(12, 0, +1),  
                Cocell(13, 1, -1),  
                Cocell(13, 2, +1),  
                Cocell(14, 3, -1),  
                Cocell(14, 4, +1),  
                Cocell(15, 5, -1),  
                Cocell(15, 6, +1)
            };

            Body* body = new Body(cells, cocells);
            return body;
        }

        Body* createSheetRoundRect(const Vec3d& lowerLeft, const Vec3d& upperRight, double cornerRadius) {
            // A 4-sided rectangle on the z=0 plane, with interior
            Body* body = createWireRoundRect(lowerLeft, upperRight, cornerRadius);
            // Add a face cell for the interior
            Cell faceCell(Pla3d(Vec3d(0,0,0), Vec3d(0,0,1))); // face on z=0 plane
            size_t faceCellIndex = body->addCell(faceCell);
            // Add cocells connecting the face to its boundary edges
            body->addCocell(Cocell(faceCellIndex, 8, +1));
            body->addCocell(Cocell(faceCellIndex, 9, +1));
            body->addCocell(Cocell(faceCellIndex, 10, +1));
            body->addCocell(Cocell(faceCellIndex, 11, +1));
            body->addCocell(Cocell(faceCellIndex, 12, +1));
            body->addCocell(Cocell(faceCellIndex, 13, +1));
            body->addCocell(Cocell(faceCellIndex, 14, +1));
            body->addCocell(Cocell(faceCellIndex, 15, +1));
            body->updateGraph();

            return body;
        }

        
    };
};

