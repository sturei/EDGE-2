#pragma once
#include "features/feature.h"
#include "document/model.h"

// Note: FeatureModel owns its features.
namespace e2 {
    class FeatureModel : public Model {
    public:
        FeatureModel() : Model() {}
        ~FeatureModel();
        const std::vector<Feature*> features() const { return m_features; }
        size_t numFeatures() const { return m_features.size(); }
        Feature& feature(size_t index) { return *m_features[index]; }
        const Feature& feature(size_t index) const { return *m_features[index]; }
        size_t addFeature(Feature* feature);
        void removeFeature(size_t index);
        void print(std::ostream& os) const override;
    private:
        std::vector<Feature*> m_features; 
    };

};

