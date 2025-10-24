#pragma once
#include "document/model.h"
#include "gui/gitem.h"

namespace e2 {
    class GuiModel : public Model {
    public:
        GuiModel() : Model() {}
        ~GuiModel() override {} 

        size_t numGItems() const { return m_drawList.size(); }
        GItem* gItem(size_t index) { return m_drawList[index]; }
        const GItem* gItem(size_t index) const { return m_drawList[index]; }
        size_t addGItem(GItem* item);
        void removeGItem(size_t index);
        void print(std::ostream& os) const override;

    private:
        std::vector<GItem*> m_drawList;
    };
};

