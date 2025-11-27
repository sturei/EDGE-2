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

        /** A 2d object (really an infinite 3d object) consisting of the interior points of a rectangle, defined as the intersection of four half-planes */
        FObject* rectangle(const Vec3d& lowerLeft, const Vec3d& upperRight) {
            // Create a frep object representing a rectangle defined by lower-left and upper-right corners
            // The rectangle can be represented as the intersection of four half-planes

            // Define the four half-planes as FNodes
            // Left half-plane: x >= lowerLeft.x()
            // Right half-plane: x <= upperRight.x()
            // Bottom half-plane: y >= lowerLeft.y()
            // Top half-plane: y <= upperRight.y()

            Pla3d leftPlane(Vec3d(lowerLeft.x(), 0, 0), Vec3d(-1, 0, 0));
            Pla3d rightPlane(Vec3d(upperRight.x(), 0, 0), Vec3d(1, 0, 0));
            Pla3d bottomPlane(Vec3d(0, lowerLeft.y(), 0), Vec3d(0, -1, 0));
            Pla3d topPlane(Vec3d(0, upperRight.y(), 0), Vec3d(0, 1, 0));   

            std::vector<Function*> functions = {
                new FHalfSpace(leftPlane),
                new FHalfSpace(rightPlane),
                new FHalfSpace(bottomPlane),
                new FHalfSpace(topPlane),
                new FIntersection()
            };

            std::vector<FNode> nodes = {
                FNode(0), // Left
                FNode(1), // Right
                FNode(2), // Bottom
                FNode(3), // Top
                FNode(4)  // Intersection node
            };

            std::vector<FArg> args = {
                FArg(0, 4), // Intersection node arg 1: Left
                FArg(1, 4), // Intersection node arg 2: Right
                FArg(2, 4), // Intersection node arg 3: Bottom
                FArg(3, 4)  // Intersection node arg 4: Top
            };

            FNodeIndex rootIndex = 4; // The intersection node is the root

            FObject* rectangleObject = new FObject(functions, nodes, args, rootIndex);
            return rectangleObject;
        }

        /** A 3d object consisting of the interior points of a block with specified width, height and depth, centered on the origin */
        FObject* block(double width, double height, double depth) {
            Vec3d halfSize(width / 2.0, height / 2.0, depth / 2.0);

            FObject* infinitePrism = rectangle(-halfSize, halfSize);
            Pla3d frontPlane(halfSize, Vec3d(0, 0, 1));
            Pla3d backPlane(-halfSize, Vec3d(0, 0, -1));

            std::vector<Function*> functions = {
                new FFObject(*infinitePrism),
                new FHalfSpace(frontPlane),
                new FHalfSpace(backPlane),
                new FIntersection()
            };

            std::vector<FNode> nodes = {
                FNode(0), // Infinite prism
                FNode(1), // Front half-space
                FNode(2), // Back half-space
                FNode(3)  // Intersection node
            };
            std::vector<FArg> args = {
                FArg(0, 3), // Intersection node arg 1: Infinite prism
                FArg(1, 3), // Intersection node arg 2: Front half-space
                FArg(2, 3)  // Intersection node arg 3: Back half-space
            };
            FNodeIndex rootIndex = 3; // The intersection node is the root
            FObject* blockObject = new FObject(functions, nodes, args, rootIndex);
            return blockObject; 
        }

        /** A 2d object (really an infinite 3d object) consisting of the interior points of an extruded profile (represented as a brep) */
        FObject* infiniteExtrudedProfile(const Body& profileBody) {
            std::vector<Function*> functions = {
                new FProfileSDF(profileBody)
            };
            std::vector<FNode> nodes = {
                FNode(0),
            };
            std::vector<FArg> args = {
            };
            FNodeIndex rootIndex = 0; // The profile SDF node is the root
            FObject* infiniteExtrudedProfile = new FObject(functions, nodes, args, rootIndex);
            return infiniteExtrudedProfile;
        }

        /** A 3d object consisting of the interior points of an extrusion of the given profile by a specified depth */
        FObject* extrudedProfile(const Body& profileBody, double depth) {
            CellIndex profileFace;
            Pla3d profilePlane;

            if (!getProfileFace(profileBody, profileFace)) {
                return nullptr; // could not find profile face
            }
            if (!getProfilePlane(profileFace, profileBody, profilePlane)) {
                return nullptr; // could not determine profile plane
            }

            Pla3d basePlane = Pla3d(profilePlane.position() - depth/2 * profilePlane.normal(), profilePlane.normal());
            Pla3d topPlane = Pla3d(profilePlane.position() + depth/2 * profilePlane.normal(), profilePlane.normal());
        
            std::vector<Function*> functions = {
                new FProfileSDF(profileBody),
                new FHalfSpace(topPlane),
                new FHalfSpace(basePlane),
                new FIntersection(),
                new FComplement()        // to invert the half-space defined by the base plane (for fun - could easily reverse the base plane normal instead)
            };
            std::vector<FNode> nodes = {
                FNode(0),     // Profile SDF
                FNode(1),     // Top half-space   
                FNode(2),     // Base half-space
                FNode(3),     // Intersection node
                FNode(4)      // Complement node
            };
            std::vector<FArg> args = {
                FArg(0, 3),  // Intersection node arg 1: Profile SDF
                FArg(1, 3),  // Intersection node arg 2: Top half-space
                FArg(2, 4),  // Complement node arg: Base half-space 
                FArg(4, 3)   // Intersection node arg 3: Complement of base half-space
            };
            FNodeIndex rootIndex = 3; // The intersection node is the root
            FObject* extrudedProfile = new FObject(functions, nodes, args, rootIndex);
            return extrudedProfile;
        }

        /** A 3d object consisting of the interior points of an extrusion of the given profile by specified depth, exact SDF everywhere */
        FObject* extrudedProfileExact(const Body& profileBody, double depth) {
            CellIndex profileFace;
            Pla3d profilePlane;

            if (!getProfileFace(profileBody, profileFace)) {
                return nullptr; // could not find profile face
            }
            if (!getProfilePlane(profileFace, profileBody, profilePlane)) {
                return nullptr; // could not determine profile plane
            }

            std::vector<Function*> functions = {
                new FProfileSDF(profileBody),
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
            return extrudedProfileExact;    
        }
    }
}

