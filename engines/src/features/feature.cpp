#include <iostream>

#include "features/feature.h"

/***
 */

namespace e2 {

    //
    // Feature
    //

    Feature::Feature(const std::string& pathname, const std::string& displayName, FeatureType featureType, FeatureEffect featureEffect)
        : m_pathname(pathname), m_displayName(displayName), m_featureType(featureType), m_featureEffect(featureEffect)
    {}

    void Feature::print(std::ostream& os) const {
        os << "Feature(pathname=" << m_pathname 
           << ", displayName=" << m_displayName 
           << ", featureType=" << m_featureType
           << ", featureEffect=" << m_featureEffect << ")";
    }

    std::ostream& operator<<(std::ostream& os, const Feature& feature) {
        feature.print(os);
        return os;
    }

    //
    // Primitive
    //

    Primitive::Primitive(
        const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
        PrimitiveType primitiveType, const Vec3d& position, const Vec3d& rotation)
        : Feature(pathname, displayName, FeatureType::PRIMITIVE, featureEffect), 
          m_primitiveType(primitiveType), m_position(position), m_rotation(rotation) {} 

    //  
    // Block
    //

    Block::Block(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
        double width, double height, double depth)
        : Primitive(pathname, displayName, featureEffect, PrimitiveType::BLOCK, Vec3d(0, 0, 0), Vec3d(0, 0, 0)), 
          m_width(width), m_height(height), m_depth(depth) {}   

    void Block::print(std::ostream& os) const {
        os << "Block(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureType=" << featureType()
           << ", featureEffect=" << featureEffect()
           << ", width=" << m_width
           << ", height=" << m_height
           << ", depth=" << m_depth << ")";
    }

    //
    // Sphere
    //  

    Sphere::Sphere(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, double radius)
        : Primitive(pathname, displayName, featureEffect, PrimitiveType::SPHERE, Vec3d(0, 0, 0), Vec3d(0, 0, 0)), 
          m_radius(radius) {}

    void Sphere::print(std::ostream& os) const {
        os << "Sphere(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureType=" << featureType()
           << ", featureEffect=" << featureEffect()
           << ", radius=" << m_radius << ")";
    }   

    //
    // Cylinder
    //
    Cylinder::Cylinder(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, double radius, double depth)
        : Primitive(pathname, displayName, featureEffect, PrimitiveType::CYLINDER, Vec3d(0, 0, 0), Vec3d(0, 0, 0)), 
          m_radius(radius), m_depth(depth) {}

    void Cylinder::print(std::ostream& os) const {
        os << "Cylinder(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureType=" << featureType()
           << ", featureEffect=" << featureEffect()
           << ", radius=" << m_radius
           << ", depth=" << m_depth << ")";
    }

} // namespace e2