#include "frep.deprecated/frepModel.h"
#include "document/model.h"

/**
 * A FRepModel is a collection of FObjects. The FRepModel takes ownership of any objects added to it,
 */

namespace e2 {
    
    FRepModel::~FRepModel() {
        for (FObject* object : m_objects) {
            delete object;
        }
    }

    size_t FRepModel::addObject(FObject* object) {
        m_objects.push_back(object);
        return m_objects.size() - 1;
    }
    void FRepModel::removeObject(size_t index) {
        if (index < m_objects.size()) {
            delete m_objects[index]; 
            m_objects.erase(m_objects.begin() + index);
        }
    }
    void FRepModel::print(std::ostream& os) const {
        os << "FRepModel with " << m_objects.size() << " objects." << std::endl;
        for (size_t i = 0; i < m_objects.size(); ++i) {
            os << "  Object " << i << ": " << *m_objects[i] << std::endl;
        }
    }
};

