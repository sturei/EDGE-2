#pragma once

/**
 * Vec3d represents a 3D vector assumed to be a point or direction in 3D space.
 * Implementation notes:
 * All methods  honour value semantics.
 * Operator== uses exact equality of components. Fuzzy equality will be implemented later as a non-member function.
*/
namespace e2 {
    class Vec3d {
        public:
            Vec3d() {}
            Vec3d(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}
            double x() const { return m_x; }
            double y() const { return m_y; }
            double z() const { return m_z; }
            bool operator==(const Vec3d& other) const {
                return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
            }
        private:
            double m_x=0, m_y=0, m_z=0;
        // TODO: Add methods as needed
    };
};

