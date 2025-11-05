#pragma once
#include <iostream>
#include "utils/vec3d.h"

/**
* A sphere represented by center and radius
*/

namespace e2 {
    class Sph3d {
        public:
            Sph3d() {}
            Sph3d(const Vec3d& center, double radius) : m_center(center), m_radius(radius) {}
            Vec3d center() const { return m_center; }
            double radius() const { return m_radius; }
            friend std::ostream& operator<<(std::ostream& os, const Sph3d& s) {
                os << "Center: " << s.m_center << ", Radius: " << s.m_radius;
                return os;
            }
            bool operator==(const Sph3d& other) const {
                return m_center == other.m_center && m_radius == other.m_radius;
            }
        private:
            Vec3d m_center = Vec3d(0,0,0);
            double m_radius = 1.0;
    };
};
