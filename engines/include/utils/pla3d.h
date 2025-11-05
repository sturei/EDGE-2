#pragma once
#include <iostream>
#include "utils/vec3d.h"

/**
* An infinite plane represented by position and normal
*/

namespace e2 {
    class Pla3d {
        public:
            Pla3d() {}
            Pla3d(const Vec3d& position, const Vec3d& normal) : m_position(position), m_normal(normal) {}
            Vec3d position() const { return m_position; }
            Vec3d normal() const { return m_normal; }
            friend std::ostream& operator<<(std::ostream& os, const Pla3d& p) {
                os << "Position: " << p.m_position << ", Normal: " << p.m_normal;
                return os;
            }
            bool operator==(const Pla3d& other) const {
                return m_position == other.m_position && m_normal == other.m_normal;
            }
        private:
            Vec3d m_position = Vec3d(0,0,0);
            Vec3d m_normal = Vec3d(0,0,1);
    };
};
