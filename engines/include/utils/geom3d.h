
#pragma once
#include "utils/vec3d.h"
#include "utils/ray3d.h"
#include "utils/sph3d.h"
#include "utils/pla3d.h"
#include "utils/cir3d.h"


/**
 * Unified representation of simple geometries in 3D space.
 * The style is "facetted fatty class with value semantics".
 * "Fatty" means the Geom3d class has enough storage to represent any of the primitive geometric types directly. This costs a bit more in memory, but should 
 * be a performance win because of better data locality. (More complex geometries with highly variable memory requirements (e.g. NURBS, meshes) will be represented differently.)
 * "Facetted" means it has multiple interfaces depending on the type and capabilities of the geometry.
 * The simplest facets correspond to the primitive geometry types: point, line, circle, plane, sphere etc.
 * In future the facets could be extended to provide interfaces like  "isDirectional", "isPlanar", "isAxial" etc to enable client code to query 
 * for such characteristics in a generic way.
 * "Value semantics" means you can copy it, assign it, pass it by value or by reference, but you can't modify it.
*/

namespace e2 {

    enum class Geom3dType {
        POINT,
        LINE,
        CIRCLE,
        PLANE,
        SPHERE,
        RUBBER          // This means "no geometry". The other fields can be set to any values - they should be ignored for rubber geometries.
    };

    inline static std::ostream& operator<<(std::ostream& os, Geom3dType type) {
        switch (type) {
            case Geom3dType::POINT:
                os << "POINT";
                break;
            case Geom3dType::LINE:
                os << "LINE";
                break;
            case Geom3dType::CIRCLE:
                os << "CIRCLE";
                break;
            case Geom3dType::PLANE:
                os << "PLANE";
                break;
            case Geom3dType::SPHERE:
                os << "SPHERE";
                break;
            case Geom3dType::RUBBER:
                os << "RUBBER";
                break;
            default:
                os << "UNKNOWN";
                break;
        }
        return os;
    }

    class Geom3d {
        private:
            Vec3d m_pos = Vec3d(0,0,0); // position of the geometry in 3D space
            Vec3d m_dir = Vec3d(1,0,0); // primary direction/orientation of the geometry in 3D space, if applicable.
            Vec3d m_dir2 = Vec3d(0,1,0); // secondary direction/orientation of the geometry in 3D space, if applicable.
            double m_scale = 1; // primary size/extent of the geometry in 3D space, if applicable
            double m_scale2 = 1; // secondary size/extent of the geometry in 3D space, if applicable
            Geom3dType m_type = Geom3dType::RUBBER; // type of geometry represented  
        public:
            // constructors
            Geom3d(Geom3dType type, const Vec3d& pos, const Vec3d& dir, const Vec3d& dir2, double scale, double scale2)
                : m_type(type), m_pos(pos), m_dir(dir), m_dir2(dir2), m_scale(scale), m_scale2(scale2) {}

            Geom3d(const Vec3d& point) 
                : Geom3d(Geom3dType::POINT, point, Vec3d(1,0,0), Vec3d(0,1,0), 1, 1) {}
            Geom3d(const Ray3d& line) 
                : Geom3d(Geom3dType::LINE, line.position(), line.direction(), Vec3d(0,1,0), 1, 1) {}
            Geom3d(const Cir3d& circle) 
                : Geom3d(Geom3dType::CIRCLE, circle.center(), circle.normal(), Vec3d(0,1,0), circle.radius(), 1) {}
            Geom3d(const Pla3d& plane) 
                : Geom3d(Geom3dType::PLANE, plane.position(), plane.normal(), Vec3d(0,1,0), 1, 1) {}
            Geom3d(const Sph3d& sphere) 
                : Geom3d(Geom3dType::SPHERE, sphere.center(), Vec3d(1,0,0), Vec3d(0,1,0), sphere.radius(), 1) {}
            Geom3d() 
                : Geom3d(Geom3dType::RUBBER, Vec3d(0,0,0), Vec3d(1,0,0), Vec3d(0,1,0), 1, 1) {}


            // accessors
            const Vec3d& position() const { return m_pos; }
            const Vec3d& direction() const { return m_dir; }
            const Vec3d& direction2() const { return m_dir2; }
            double scale() const { return m_scale; }
            double scale2() const { return m_scale2; } 
            Geom3dType type() const { return m_type; }

            // utilities
            int dimensionality() const {
                switch (m_type) {
                    case Geom3dType::POINT: return 0;
                    case Geom3dType::LINE: return 1;
                    case Geom3dType::CIRCLE: return 1;
                    case Geom3dType::PLANE: return 2;
                    case Geom3dType::SPHERE: return 2;
                    case Geom3dType::RUBBER: return -1;
                    default: return -1;
                }
            }

            // facets
            bool isPoint(Vec3d& point) const { 
                if (m_type == Geom3dType::POINT) {
                    point = m_pos;
                    return true;
                }
                return false;
            }
            bool isLine(Ray3d& line) const { 
                if (m_type == Geom3dType::LINE) {
                    line = Ray3d(m_pos, m_dir);
                    return true;
                }
                return false;
            }
            bool isCircle(Cir3d& circle) const { 
                if (m_type == Geom3dType::CIRCLE) {
                    circle = Cir3d(m_pos, m_scale, m_dir);
                    return true;
                }
                return false;
            }
            bool isPlane(Pla3d& plane) const { 
                if (m_type == Geom3dType::PLANE) {
                    plane = Pla3d(m_pos, m_dir);
                    return true;
                }
                return false;
            }
            bool isSphere(Sph3d& sphere) const { 
                if (m_type == Geom3dType::SPHERE) {
                    sphere = Sph3d(m_pos, m_scale);
                    return true;
                }
                return false;
            } 
            bool isRubber() const { 
                return m_type == Geom3dType::RUBBER; 
            }

            // stream output
            friend std::ostream& operator<<(std::ostream& os, const Geom3d& g) {
                os << "Geom3d( type=" << g.m_type
                   << ", pos=" << g.m_pos
                   << ", dir=" << g.m_dir
                   << ", dir2=" << g.m_dir2
                   << ", scale=" << g.m_scale
                   << ", scale2=" << g.m_scale2 << ")";
                return os;
            }
    };

};

