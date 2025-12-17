#pragma once
#include "brep/brepModel.h"
#include "features/featureModel.h"

namespace e2 {
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

