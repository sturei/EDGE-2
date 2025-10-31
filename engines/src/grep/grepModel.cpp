#include "grep/grepModel.h"
#include "document/model.h"

/**
 * A GRepModel holds a drawlist, which is just a collection of graphical items.
 */

namespace e2 {
    size_t GRepModel::addGItem(GItem* item) {
        m_drawList.push_back(item);
        return m_drawList.size() - 1;
    }
    void GRepModel::removeGItem(size_t index) {
        // simple erase for now. Can do something more sophisticated later (e.g. swap-and-pop) if needed.
        // Note that this invalidates all indexes after the removed item.
        if (index < m_drawList.size()) {
            m_drawList.erase(m_drawList.begin() + index);
        }
    }
    void GRepModel::print(std::ostream& os) const {
        os << "GRepModel with " << m_drawList.size() << " items." << std::endl;
        for (size_t i = 0; i < m_drawList.size(); ++i) {
            os << "  Item " << i << ": " << *m_drawList[i] << std::endl;
        }
    }
};

