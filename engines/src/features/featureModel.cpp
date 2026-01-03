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
        if (!findFeature(feature->pathname())) {
            // only add if a feature with the same pathname doesn't already exist   
            m_features.push_back(feature);
        }
        return m_features.size() - 1;
    }

    void FeatureModel::removeFeature(size_t index) {
        if (index < m_features.size()) {
            delete m_features[index]; 
            m_features.erase(m_features.begin() + index);
        }
    }

    Feature* FeatureModel::findFeature(const std::string& pathname) const {
        for (Feature* feature : m_features) {
            if (feature->pathname() == pathname) {
                return feature;
            }
        }
        return nullptr;
    }

    std::vector<const Feature*> FeatureModel::findModifyingFeatures(const Feature* targetFeature) const {
        std::vector<const Feature*> modifyingFeatures;
        for (const Feature* feature : m_features) {
            if (feature->featureEffect() == FeatureEffect::MODIFY) {
                if (const Fill* fillFeature = dynamic_cast<const Fill*>(feature)) {
                    if (fillFeature->targetPathName() == targetFeature->pathname()) {
                        modifyingFeatures.push_back(feature);
                    }
                }
            }
        }
        return modifyingFeatures;
    }


    void FeatureModel::clearFeatures() {
        for (Feature* feature : m_features) {
            delete feature;
        }
        m_features.clear();
    }

    void FeatureModel::print(std::ostream& os) const {
        os << "FeatureModel with " << m_features.size() << " features." << std::endl;
        for (size_t i = 0; i < m_features.size(); ++i) {
            os << "  Feature " << i << ": " << *m_features[i] << std::endl;
        }
    }
};

