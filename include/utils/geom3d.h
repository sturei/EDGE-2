
#pragma once
#include "utils/vec3d.h"

/**
 * Representation of simple geometries in 3D space.
 * Implementation notes:
 * The style is "fat base class, thin interfaces" - all the types have identical memory layout, yet each type has its own methods.
 * This style pays a penalty in terms of memory usage, but should be a performance win because of better data locality, assuming 
 * that related data will be stored together.
 * I think the data is rich enough to represent: point, line, circle, ellipse, plane, sphere, cylinder, cone, torus with a defined parameterization on each.
 * More complex geometries, e.g. swept surfaces, meshes, NURBS, will be represented differently (probably with a "rep" concept in the owning cell).
 * All methods honour value semantics.
 * TODO: support parametrizations where applicable (probably using non-member functions to avoid polluting the classes). Will need to add secondary directions and scales where applicable.
 * Probably need to add a type field.
*/

namespace e2 {
    class Geom3d {
        private:
            Vec3d m_pos; // position of the geometry in 3D space
            Vec3d m_dir; // primary direction/orientation of the geometry in 3D space, if applicable. Not necessarily a unit vector.
            Vec3d m_dir2; // secondary direction/orientation of the geometry in 3D space, if applicable. Not necessarily a unit vector.
            double m_scale = 1; // primary size/extent of the geometry in 3D space, if applicable
            double m_scale2 = 1; // secondary size/extent of the geometry in 3D space, if applicable
        public:
            Geom3d() : m_pos(0,0,0), m_dir(1,0,0), m_dir2(0,1,0), m_scale(1), m_scale2(1) {}
            Geom3d(const Vec3d& pos, const Vec3d& dir, const Vec3d& dir2, double scale, double scale2)
                : m_pos(pos), m_dir(dir), m_dir2(dir2), m_scale(scale), m_scale2(scale2) {}  
            const Vec3d& position() const { return m_pos; }
            const Vec3d& direction() const { return m_dir; }
            const Vec3d& direction2() const { return m_dir2; }
            double scale() const { return m_scale; }
            double scale2() const { return m_scale2; } 
            friend std::ostream& operator<<(std::ostream& os, const Geom3d& g) {
                os << "Geom3d(pos=" << g.m_pos
                   << ", dir=" << g.m_dir
                   << ", dir2=" << g.m_dir2
                   << ", scale=" << g.m_scale
                   << ", scale2=" << g.m_scale2 << ")";
                return os;
            }
    };

    class Plane3d : public Geom3d {
        public:
            Plane3d() : Geom3d() {}
            Plane3d(const Vec3d& pos, const Vec3d& normal)
                : Geom3d(pos, normal, Vec3d(0,0,0), 1, 1) {}  
            const Vec3d& position() const { return Geom3d::position(); }
            const Vec3d& normal() const { return Geom3d::direction(); }    
            friend std::ostream& operator<<(std::ostream& os, const Plane3d& p) {
                os << "Plane3d(pos=" << p.position()
                   << ", normal=" << p.normal() << ")";
                return os;
             }
    };

    class Sphere3d : public Geom3d {
        public:
            Sphere3d() : Geom3d() {}
            Sphere3d(const Vec3d& center, double radius)
                : Geom3d(center, Vec3d(0,0,0), Vec3d(0,0,0), radius, 1) {}  
            const Vec3d& center() const { return Geom3d::position(); }
            double radius() const { return Geom3d::scale(); }
            friend std::ostream& operator<<(std::ostream& os, const Sphere3d& s) {
                os << "Sphere3d(center=" << s.center()
                   << ", radius=" << s.radius() << ")";
                return os;
             }          
    };

    class Point3d : public Geom3d {
        public:
            Point3d() : Geom3d() {}
            Point3d(const Vec3d& pos)
                : Geom3d(pos, Vec3d(0,0,0), Vec3d(0,0,0), 1, 1) {}  
            const Vec3d& position() const { return Geom3d::position(); }
            friend std::ostream& operator<<(std::ostream& os, const Point3d& p) {
                os << "Point3d(pos=" << p.position() << ")";
                return os;
            }   
    };

    class Line3d : public Geom3d {
        public:
            Line3d() : Geom3d() {}
            Line3d(const Vec3d& pos, const Vec3d& dir)
                : Geom3d(pos, dir, Vec3d(0,0,0), 1, 1) {}  
            const Vec3d& position() const { return Geom3d::position(); }
            const Vec3d& direction() const { return Geom3d::direction(); }
            friend std::ostream& operator<<(std::ostream& os, const Line3d& l) {
                os << "Line3d(pos=" << l.position()
                   << ", dir=" << l.direction() << ")";
                return os;
             }          
    };

    class Circle3d : public Geom3d {
        public:
            Circle3d() : Geom3d() {}
            Circle3d(const Vec3d& center, const Vec3d& normal, double radius)
                : Geom3d(center, normal, Vec3d(0,0,0), radius, 1) {}  
            const Vec3d& center() const { return Geom3d::position(); }
            const Vec3d& normal() const { return Geom3d::direction(); }          
            double radius() const { return Geom3d::scale(); }
            friend std::ostream& operator<<(std::ostream& os, const Circle3d& c) {
                os << "Circle3d(center=" << c.center()
                   << ", normal=" << c.normal()
                   << ", radius=" << c.radius() << ")";
                return os;
             }          
    };

    inline const Plane3d& geomAsPlane(const Geom3d& g){
        return static_cast<const Plane3d&>(g);
    }
    inline const Sphere3d& geomAsSphere(const Geom3d& g){
        return static_cast<const Sphere3d&>(g);
    }
    inline const Point3d& geomAsPoint(const Geom3d& g){
        return static_cast<const Point3d&>(g);
    }
    inline const Line3d& geomAsLine(const Geom3d& g){
        return static_cast<const Line3d&>(g);
    }
    inline const Circle3d& geomAsCircle(const Geom3d& g){
        return static_cast<const Circle3d&>(g);
    }
};

