#pragma once
#include <iostream>
#include "utils/vec3d.h"

/** A circle represented by centre, radius, normal and local x-axis */

namespace e2 {
    class Cir3d {
        public:
            Cir3d() {}
            Cir3d(const Vec3d& center, double radius, const Vec3d& normal, const Vec3d& xaxis) : m_center(center), m_radius(radius), m_normal(normal), m_xaxis(xaxis) {}
            Cir3d(const Vec3d& center, double radius, const Vec3d& normal) : m_center(center), m_radius(radius), m_normal(normal), m_xaxis(normal.ortho()) {}
            Vec3d center() const { return m_center; }
            double radius() const { return m_radius; }
            Vec3d normal() const { return m_normal; }
            Vec3d xaxis() const { return m_xaxis; }
            Vec3d yaxis() const { return m_normal.cross(m_xaxis); }
            friend std::ostream& operator<<(std::ostream& os, const Cir3d& c) {
                os << "Center: " << c.m_center << ", Radius: " << c.m_radius << ", Normal: " << c.m_normal << ", XAxis: " << c.m_xaxis << ", YAxis: " << c.yaxis();
                return os;
            }
            bool operator==(const Cir3d& other) const {
                return m_center == other.m_center && m_radius == other.m_radius && m_normal == other.m_normal && m_xaxis == other.m_xaxis;
            }
        private:
            Vec3d m_center = Vec3d(0,0,0);
            double m_radius = 1.0;
            Vec3d m_normal = Vec3d(0,0,1);
            Vec3d m_xaxis = Vec3d(1,0,0); // local x axis
    };
};

