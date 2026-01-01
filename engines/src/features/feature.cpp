#include <iostream>

#include "features/feature.h"

/***
 */

namespace e2 {

    //
    // Feature
    //

    Feature::Feature(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation )
        : m_pathname(pathname), m_displayName(displayName), m_featureEffect(featureEffect), m_position(position), m_rotation(rotation) {
    }

    std::ostream& operator<<(std::ostream& os, const Feature& feature) {
        feature.print(os);
        return os;
    }

    //
    // Primitive
    //

    Primitive::Primitive(
        const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation)
        : Feature(pathname, displayName, featureEffect, position, rotation) {
    } 
        
    //  
    // Block
    //

    Block::Block(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation,
        double width, double height, double depth)
        : Primitive(pathname, displayName, featureEffect, position, rotation), 
          m_width(width), m_height(height), m_depth(depth) {    
    }   

    void Block::print(std::ostream& os) const {
        os << "Block(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", position=" << position()
           << ", rotation=" << rotation()
           << ", width=" << m_width
           << ", height=" << m_height
           << ", depth=" << m_depth << ")";
    }

    //
    // Sphere
    //  

    Sphere::Sphere(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation, double radius)
        : Primitive(pathname, displayName, featureEffect, position, rotation), 
          m_radius(radius) {}

    void Sphere::print(std::ostream& os) const {
        os << "Sphere(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", position=" << position()
           << ", rotation=" << rotation()
           << ", radius=" << m_radius << ")";
    }   

    //
    // Cylinder
    //

    Cylinder::Cylinder(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation, double radius, double depth)
        : Primitive(pathname, displayName, featureEffect, position, rotation), 
          m_radius(radius), m_depth(depth) {}

    void Cylinder::print(std::ostream& os) const {
        os << "Cylinder(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", position=" << position()
           << ", rotation=" << rotation()
           << ", radius=" << m_radius
           << ", depth=" << m_depth << ")";
    }

    //
    // Workplane
    //

    Workplane::Workplane(const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation)
        : Feature(pathname, displayName, FeatureEffect::ADD, position, rotation) {
    }   

    void Workplane::print(std::ostream& os) const {
        os << "Workplane(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", position=" << position()
           << ", rotation=" << rotation() << ")";
    }

    //
    // Feature2D
    //

    Feature2D::Feature2D(
        const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation,
        const std::string& workplanePathName, const Vec3d& position2D, double rotation2D)
        : Feature(pathname, displayName, FeatureEffect::ADD, position, rotation), 
        m_workplanePathName(workplanePathName), m_position2D(position2D), m_rotation2D(rotation2D) {
    }

    //
    // Primitive2D
    //

    Primitive2D::Primitive2D(const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation,
        const std::string& workplanePathName,const Vec3d& position2D, double rotation2D)
        : Feature2D(pathname, displayName, position, rotation, workplanePathName, position2D, rotation2D) {
    }

    //
    // Rectangle2D
    //

    Rectangle2D::Rectangle2D(
        const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation,
        const std::string& workplanePathName, const Vec3d& position2D, double rotation2D, 
        double width, double height)
        : Primitive2D(pathname, displayName, position, rotation, workplanePathName, position2D, rotation2D), 
          m_width(width), m_height(height) {
    }

    void Rectangle2D::print(std::ostream& os) const {
        os << "Rectangle2D(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", position=" << position()
           << ", rotation=" << rotation()
           << ", workplanePathName=" << workplanePathName()
           << ", position2D=" << position2D()
           << ", rotation2D=" << rotation2D()
           << ", width=" << m_width
           << ", height=" << m_height << ")";
    }   
    
    //
    // Circle2D
    //

    Circle2D::Circle2D(
        const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation,
        const std::string& workplanePathName, const Vec3d& position2D, double rotation2D, 
        double radius)  
        : Primitive2D(pathname, displayName, position, rotation, workplanePathName, position2D, rotation2D), 
          m_radius(radius) {
    }

    void Circle2D::print(std::ostream& os) const {
        os << "Circle2D(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", position=" << position()
           << ", rotation=" << rotation()
           << ", workplanePathName=" << workplanePathName()
           << ", position2D=" << position2D()
           << ", rotation2D=" << rotation2D()
           << ", radius=" << m_radius << ")";
    }   

    //
    // RoundRect2D
    //
    
    RoundRect2D::RoundRect2D(
        const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation,
        const std::string& workplanePathName, const Vec3d& position2D, double rotation2D, 
        double width, double height, double cornerRadius)
        : Primitive2D(pathname, displayName, position, rotation, workplanePathName, position2D, rotation2D), 
          m_width(width), m_height(height), m_cornerRadius(cornerRadius) {
    }   

    void RoundRect2D::print(std::ostream& os) const {
        os << "RoundRect2D(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", position=" << position()
           << ", rotation=" << rotation()
           << ", workplanePathName=" << workplanePathName()
           << ", position2D=" << position2D()
           << ", rotation2D=" << rotation2D()
           << ", width=" << m_width
           << ", height=" << m_height
           << ", cornerRadius=" << m_cornerRadius << ")";
    }   

    //
    // Extrusion
    //

    Extrusion::Extrusion(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation,
        const std::string& profilePathName, double depth, bool doubleSided)
        : Feature(pathname, displayName, featureEffect, position, rotation), 
          m_profilePathName(profilePathName), m_depth(depth), m_doubleSided(doubleSided) {
}

    void Extrusion::print(std::ostream& os) const {
        os << "Extrusion(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", position=" << position()
           << ", rotation=" << rotation()
           << ", profilePathName=" << m_profilePathName
           << ", depth=" << m_depth
           << ", doubleSided=" << (m_doubleSided ? "true" : "false") <<  ")";
    }   

    //
    // Fill
    //

    Fill::Fill(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect,
        const Vec3d& position, const Vec3d& rotation,
        const std::string& targetPathName, FillType fillType, double cellSize)
        : Feature(pathname, displayName, featureEffect, position, rotation), 
          m_targetPathName(targetPathName), m_fillType(fillType), m_cellSize(cellSize) {
    }

    void Fill::print(std::ostream& os) const {
        os << "Fill(pathname=" << pathname()
           << ", displayName=" << displayName()
           << ", featureEffect=" << featureEffect()
           << ", position=" << position()
           << ", rotation=" << rotation()
           << ", targetPathName=" << m_targetPathName
           << ", fillType=" << m_fillType
           << ", cellSize=" << m_cellSize <<  ")";
    }   

} // namespace e2