#pragma once
#include "brep/brepModel.h"
#include "features/featureModel.h"

namespace e2 {

    class ProfileAttribute : public Attribute {
    public:
        ProfileAttribute() = default;
        ProfileAttribute(const ProfileAttribute& other) {
            m_isConstruction = other.m_isConstruction;
            m_zOffset = other.m_zOffset;
        };
        void operator=(const ProfileAttribute&) = delete;
        virtual ~ProfileAttribute() = default;
        ProfileAttribute( bool isConstruction, const Vec3d& position2D, double rotation2D, double zOffset ) 
            : m_isConstruction(isConstruction), m_position2D(position2D), m_rotation2D(rotation2D), m_zOffset(zOffset) {}  
        const Vec3d& position2D() const { return m_position2D; }
        void setPosition2D(const Vec3d& position2D) { m_position2D = position2D; }
        double rotation2D() const { return m_rotation2D; }
        void setRotation2D(double rotation2D) { m_rotation2D = rotation2D; }    
        double zOffset() const { return m_zOffset; }
        void setZOffset(double zOffset) { m_zOffset = zOffset; }
        bool isConstruction() const { return m_isConstruction; }
        void setIsConstruction(bool isConstruction) { m_isConstruction = isConstruction; }
        Attribute* clone() const override {
            return new ProfileAttribute(*this);
        }
        void print(std::ostream& os) const override {
            os << "ProfileAttribute(isConstruction=" << (m_isConstruction ? "true" : "false")
               << ", position2D=" << m_position2D
               << ", rotation2D=" << m_rotation2D
               << ", zOffset=" << m_zOffset << ")";
        }
    private:
        bool m_isConstruction = false;
        Vec3d m_position2D = Vec3d(0,0,0);
        double m_rotation2D = 0.0;
        double m_zOffset = 0.0;
    };

    class WorkplaneAttribute : public Attribute {
    public:
        WorkplaneAttribute() = default;
        WorkplaneAttribute(const WorkplaneAttribute& other) {
            m_zOffset = other.m_zOffset;
        };
        void operator=(const WorkplaneAttribute&) = delete;
        virtual ~WorkplaneAttribute() = default;
        WorkplaneAttribute(double zOffset) : m_zOffset(zOffset) {}  
        double zOffset() const { return m_zOffset; }
        void setZOffset(double zOffset) { m_zOffset = zOffset; }
        Attribute* clone() const override {
            return new WorkplaneAttribute(*this);
        }
        void print(std::ostream& os) const override {
            os << "WorkplaneAttribute(zOffset=" << m_zOffset << ")";
        }
    private:
        double m_zOffset = 0.0;
    };

    class ShapeModel : public Model {
    public:
        ShapeModel(BRepModel* sketches, BRepModel* profiles, FeatureModel* features) 
            : Model(), m_sketches(sketches), m_profiles(profiles), m_features(features) {}

        ~ShapeModel() {
            delete m_sketches;
            delete m_profiles;
            delete m_features;
        }
        BRepModel& sketches() { return *m_sketches; }
        BRepModel& profiles() { return *m_profiles; }
        FeatureModel& features() { return *m_features; }

        const BRepModel& sketches() const { return *m_sketches; }
        const BRepModel& profiles() const { return *m_profiles; }
        const FeatureModel& features() const { return *m_features; }

        void print(std::ostream& os) const override;
    private:
        BRepModel* m_sketches;
        BRepModel* m_profiles;
        FeatureModel* m_features;

    };

};

