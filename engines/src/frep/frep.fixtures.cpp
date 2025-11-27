#include "frep/fobject.h"
#include "frep/functions.h"
#include "frep/frep.fixtures.h"
#include "brep/brep.fixtures.h"
#include "brep/navigate.h"
#include "utils/pla3d.h"

/**
 * FRepFixtures provides a set of fixtures for testing or building simple freps.
 */

namespace e2 {
    namespace FRepFixtures {

        /** an Empty object */
        FObject* emptyObject() {
            std::vector<Function*> functions;
            std::vector<FNode> nodes;
            std::vector<FArg> args;
            FNodeIndex rootIndex = -1; // no root
            FObject* emptyObj = new FObject(functions, nodes, args, rootIndex);
            return emptyObj;
        }

        /** A sphere centered at the origin with the given radius */
        FObject* sphere(double radius) {
            Sph3d sphere(Vec3d(0,0,0), radius);
            std::vector<Function*> functions = {
                new FSphere(sphere)
            };
            std::vector<FNode> nodes = {
                FNode(0)
            };
            std::vector<FArg> args = {
            };
            FNodeIndex rootIndex = 0; // The sphere node is the root
            FObject* sphereObject = new FObject(functions, nodes, args, rootIndex);
            return sphereObject;
        }

        /** A block with specified width, height and depth, centered on the origin */
        FObject* block(double width, double height, double depth) {
            Vec3d halfDiag(width / 2.0, height / 2.0, depth / 2.0);

            Pla3d leftPlane(halfDiag, Vec3d(1, 0, 0));
            Pla3d rightPlane(-halfDiag, Vec3d(-1, 0, 0));    
            Pla3d topPlane(halfDiag, Vec3d(0, 1, 0));
            Pla3d bottomPlane(-halfDiag, Vec3d(0, -1, 0));
            Pla3d frontPlane(halfDiag, Vec3d(0, 0, 1));
            Pla3d backPlane(-halfDiag, Vec3d(0, 0, -1));

            std::vector<Function*> functions = {
                new FHalfSpace(leftPlane),
                new FHalfSpace(rightPlane),
                new FHalfSpace(topPlane),
                new FHalfSpace(bottomPlane),
                new FHalfSpace(frontPlane),
                new FHalfSpace(backPlane),
                new FIntersection()
            };

            std::vector<FNode> nodes = {
                FNode(0),  // Left
                FNode(1),  // Right
                FNode(2),  // Top
                FNode(3),  // Bottom
                FNode(4),  // Front
                FNode(5),  // Back
                FNode(6)   // Intersection node
            };
            std::vector<FArg> args = {
                FArg(0, 6), // Intersection node arg 1: Left half-space
                FArg(1, 6), // Intersection node arg 2: Right half-space
                FArg(2, 6), // Intersection node arg 3: Top half-space
                FArg(3, 6), // Intersection node arg 4: Bottom half-space
                FArg(4, 6), // Intersection node arg 5: Front half-space
                FArg(5, 6)  // Intersection node arg 6: Back half-space
            };
            FNodeIndex rootIndex = 6; // The intersection node is the root
            FObject* blockObject = new FObject(functions, nodes, args, rootIndex);
            return blockObject; 
        }

        /** An infinite extrusion of a rectangular profile */
        FObject* infiniteRectangle(const Vec3d& lowerLeft, const Vec3d& upperRight) {
            Body* profileBody = BRepFixtures::sheetRectangle(lowerLeft, upperRight);
            std::vector<Function*> functions = {
                new FProfileSDF(*profileBody)
            };
            std::vector<FNode> nodes = {
                FNode(0),   // Profile SDF
            };
            std::vector<FArg> args = {
            };
            FNodeIndex rootIndex = 0; // The profile SDF node is the root
            FObject* infiniteExtrudedProfile = new FObject(functions, nodes, args, rootIndex);
            delete profileBody;    // The profile SDF takes a copy of the body, so we can delete this now
            return infiniteExtrudedProfile;
        }

        /* An extruded infinite rectangle capped by half-planes */
        FObject* cappedRectangle(const Vec3d& lowerLeft, const Vec3d& upperRight, double depth) {
            FObject* infinitePrism = infiniteRectangle(lowerLeft, upperRight);
            Pla3d frontPlane(Vec3d(0,0,depth/2), Vec3d(0, 0, 1));
            Pla3d backPlane(Vec3d(0,0,-depth/2), Vec3d(0, 0, 1));    // reversed the normal to point inwards, to excercise complement operation

            std::vector<Function*> functions = {
                new FFObject(*infinitePrism),
                new FHalfSpace(frontPlane),
                new FHalfSpace(backPlane),
                new FComplement(),
                new FIntersection()
            };
            std::vector<FNode> nodes = {
                FNode(0), // Infinite prism
                FNode(1), // Front half-space
                FNode(2), // Back half-space
                FNode(3), // Complement node
                FNode(4)  // Intersection node
            };
            std::vector<FArg> args = {
                FArg(0, 4), // Intersection node arg 1: Infinite prism
                FArg(1, 4), // Intersection node arg 2: Front half-space
                FArg(3, 4), // Intersection node arg 3: Complement of back half-space
                FArg(2, 3)  // Complement node arg: Back half-space
            };
            FNodeIndex rootIndex = 4; // The intersection node is the root
            FObject* blockObject = new FObject(functions, nodes, args, rootIndex);
            delete infinitePrism; // The FFObject takes a copy of the object, so we can delete this now
            return blockObject; 
        }

        /** An extruded rectangle represented using a specific signed distance function for extruded profiles */
        FObject* extrudedRectangle(const Vec3d& lowerLeft, const Vec3d& upperRight, double depth) {
            Body* profileBody = BRepFixtures::sheetRectangle(lowerLeft, upperRight);
            std::vector<Function*> functions = {
                new FProfileSDF(*profileBody),
                new FExtrusionSDF(depth)
            };
            std::vector<FNode> nodes = {
                FNode(0),     // Profile SDF
                FNode(1),     // Extrusion operator
            };
            std::vector<FArg> args = {
                FArg(0, 1),  // Extrusion arg: Profile SDF
            };
            FNodeIndex rootIndex = 1; // The extrusion operator is the root
            FObject* extrudedProfileExact = new FObject(functions, nodes, args, rootIndex);
            delete profileBody;    // The profile SDF takes a copy of the body, so we can delete this now
            return extrudedProfileExact;    
        }
    }
}

