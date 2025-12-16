#pragma once
#include <iostream>
#include "utils/vec3d.h"

/**
* An 3D transform
*/

namespace e2 {
    class Tfm3d {
        public:
            Tfm3d() {}
            Tfm3d(const Vec3d& position, const Vec3d& angles) : m_position(position), m_angles(angles) {}
            Vec3d position() const { return m_position; }
            Vec3d angles() const { return m_angles; }
            friend std::ostream& operator<<(std::ostream& os, const Tfm3d& p) {
                os << "Position: " << p.m_position << ", Angles: " << p.m_angles;
                return os;
            }
            bool operator==(const Tfm3d& other) const {
                return m_position == other.m_position && m_angles == other.m_angles;
            }
        private:
            Vec3d m_position = Vec3d(0,0,0);
            Vec3d m_angles = Vec3d(0,0,0);
    };
};
