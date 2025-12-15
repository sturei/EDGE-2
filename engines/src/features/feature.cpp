#include <iostream>

#include "features/feature.h"

/***
 */

namespace e2 {

    //
    // Feature
    //

    Feature::Feature(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect)
        : m_pathname(pathname), m_displayName(displayName), m_featureEffect(featureEffect){
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
        const Vec3d& position, const Vec3d& rotation)
        : Feature(pathname, displayName, featureEffect), 
          m_position(position), m_rotation(rotation) {
    } 
        
    //  
    // Block
    //

    Block::Block(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
        double width, double height, double depth)
        : Primitive(pathname, displayName, featureEffect, Vec3d(0, 0, 0), Vec3d(0, 0, 0)), 
          m_width(width), m_height(height), m_depth(depth) {    
    }   

    void Block::print(std::ostream& os) const {
        os << "Block(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", width=" << m_width
           << ", height=" << m_height
           << ", depth=" << m_depth << ")";
    }

    //
    // Sphere
    //  

    Sphere::Sphere(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, double radius)
        : Primitive(pathname, displayName, featureEffect, Vec3d(0, 0, 0), Vec3d(0, 0, 0)), 
          m_radius(radius) {}

    void Sphere::print(std::ostream& os) const {
        os << "Sphere(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", radius=" << m_radius << ")";
    }   

    //
    // Cylinder
    //

    Cylinder::Cylinder(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, double radius, double depth)
        : Primitive(pathname, displayName, featureEffect, Vec3d(0, 0, 0), Vec3d(0, 0, 0)), 
          m_radius(radius), m_depth(depth) {}

    void Cylinder::print(std::ostream& os) const {
        os << "Cylinder(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", radius=" << m_radius
           << ", depth=" << m_depth << ")";
    }

    //
    // Profile
    //

    Profile::Profile(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation )
        : Feature(pathname, displayName, featureEffect), m_position(position), m_rotation(rotation) {

        // at the moment, only ADD profiles are supported. Modify and subtract would probably work from functional standpoint, but the profile graphics (which is based on brep) is not done yet
        if (featureEffect == FeatureEffect::MODIFY) {
            std::cerr << "Profile feature with MODIFY effect not supported."    ;
        }
        if (featureEffect == FeatureEffect::SUBTRACT) {
            std::cerr << "Profile feature with SUBTRACT effect not supported.";
        }
    }

    //
    // Primitive2D
    //

    Primitive2D::Primitive2D(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
        const Vec3d& position, const Vec3d& rotation, 
        const Vec3d& position2D, double rotation2D)
        : Profile(pathname, displayName, featureEffect, position, rotation), 
          m_position2D(position2D), m_rotation2D(rotation2D) {
    }

    //
    // Rectangle2D
    //

    Rectangle2D::Rectangle2D(
        const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
        const Vec3d& position, const Vec3d& rotation, 
        const Vec3d& position2D, double rotation2D, 
        double width, double height)
        : Primitive2D(pathname, displayName, featureEffect, position, rotation, position2D, rotation2D), 
          m_width(width), m_height(height) {
    }

    void Rectangle2D::print(std::ostream& os) const {
        os << "Rectangle2D(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", width=" << m_width
           << ", height=" << m_height << ")";
    }   
    
    //
    // Circle2D
    //

    Circle2D::Circle2D(
        const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
        const Vec3d& position, const Vec3d& rotation, 
        const Vec3d& position2D, double rotation2D, 
        double radius)
        : Primitive2D(pathname, displayName, featureEffect, position, rotation, position2D, rotation2D), 
          m_radius(radius) {
    }

    void Circle2D::print(std::ostream& os) const {
        os << "Circle2D(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", radius=" << m_radius << ")";
    }   

    //
    // RoundRect2D
    //
    
    RoundRect2D::RoundRect2D(
        const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
        const Vec3d& position, const Vec3d& rotation, 
        const Vec3d& position2D, double rotation2D, 
        double width, double height, double cornerRadius)
        : Primitive2D(pathname, displayName, featureEffect, position, rotation, position2D, rotation2D), 
          m_width(width), m_height(height), m_cornerRadius(cornerRadius) {
    }   

    void RoundRect2D::print(std::ostream& os) const {
        os << "RoundRect2D(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", width=" << m_width
           << ", height=" << m_height
           << ", cornerRadius=" << m_cornerRadius << ")";
    }   

    //
    // Extrusion
    //

    Extrusion::Extrusion(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
        const std::string& profilePathName, double depth, bool doubleSided)
        : Feature(pathname, displayName, featureEffect), 
          m_profilePathName(profilePathName), m_depth(depth), m_doubleSided(doubleSided) {
}

    void Extrusion::print(std::ostream& os) const {
        os << "Extrusion(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", profilePathName=" << m_profilePathName
           << ", depth=" << m_depth
           << ", doubleSided=" << (m_doubleSided ? "true" : "false") <<  ")";
    }   

} // namespace e2