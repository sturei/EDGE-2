#include "gui/guiModel.h"
#include "document/model.h"

/**
 * A GuiModel holds the drawlist, which is just a collection of graphical items.
 */

namespace e2 {
    size_t GuiModel::addGItem(GItem* item) {
        m_drawList.push_back(item);
        return m_drawList.size() - 1;
    }
    void GuiModel::removeGItem(size_t index) {
        // simple erase for now. Can do something more sophisticated later (e.g. swap-and-pop) if needed.
        // Note that this invalidates all indexes after the removed item.
        if (index < m_drawList.size()) {
            m_drawList.erase(m_drawList.begin() + index);
        }
    }
    void GuiModel::print(std::ostream& os) const {
        os << "GuiModel with " << m_drawList.size() << " items." << std::endl;
        for (size_t i = 0; i < m_drawList.size(); ++i) {
            os << "  Item " << i << ": " << *m_drawList[i] << std::endl;
        }
    }
};

