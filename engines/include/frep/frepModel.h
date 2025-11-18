#pragma once
#include "frep/fobject.h"
#include "document/model.h"

// Note: FRepModel owns its objects.
namespace e2 {
    class FRepModel : public Model {
    public:
        FRepModel() : Model() {}
        ~FRepModel();
        const std::vector<FObject*> objects() const { return m_objects; }
        size_t numObjects() const { return m_objects.size(); }
        FObject* object(size_t index) { return m_objects[index]; }
        const FObject* object(size_t index) const { return m_objects[index]; }
        size_t addObject(FObject* object);
        void removeObject(size_t index);
        void print(std::ostream& os) const override;
    private:
        std::vector<FObject*> m_objects; 
    };

};

