#include "features/featureModel.h"
#include "document/model.h"

/**
 * A FeatureModel is a collection of Features. The FeatureModel takes ownership of any features added to it,
 */

namespace e2 {
    
    FeatureModel::~FeatureModel() {
        for (Feature* feature : m_features) {
            delete feature;
        }
    }

    size_t FeatureModel::addFeature(Feature* feature) {
        m_features.push_back(feature);
        return m_features.size() - 1;
    }
    void FeatureModel::removeFeature(size_t index) {
        if (index < m_features.size()) {
            delete m_features[index]; 
            m_features.erase(m_features.begin() + index);
        }
    }
    void FeatureModel::print(std::ostream& os) const {
        os << "FFeatureModel with " << m_features.size() << " features." << std::endl;
        for (size_t i = 0; i < m_features.size(); ++i) {
            os << "  Feature " << i << ": " << *m_features[i] << std::endl;
        }
    }
};

