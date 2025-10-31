#pragma once
#include "document/model.h"
#include "grep/gitem.h"

namespace e2 {
    class GrepModel : public Model {
    public:
        GrepModel() : Model() {}
        ~GrepModel() override {} 

        size_t numGItems() const { return m_drawList.size(); }
        GItem* gItem(size_t index) { return m_drawList[index]; }
        const GItem* gItem(size_t index) const { return m_drawList[index]; }
        virtual size_t addGItem(GItem* item);
        void removeGItem(size_t index);
        void print(std::ostream& os) const override;

    private:
        std::vector<GItem*> m_drawList;
    };
};

