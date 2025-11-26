#pragma once
#include "brep/brepModel.h"
#include "frep/frepModel.h"

namespace e2 {
    class ShapeModel : public Model {
    public:
        ShapeModel(BRepModel* sketches, BRepModel* profiles, FRepModel* objects) 
            : Model(), m_sketches(sketches), m_profiles(profiles), m_objects(objects) {}

        ~ShapeModel() {
            delete m_sketches;
            delete m_profiles;
            delete m_objects;
        }
        BRepModel& sketches() { return *m_sketches; }
        BRepModel& profiles() { return *m_profiles; }
        FRepModel& objects() { return *m_objects; }

        const BRepModel& sketches() const { return *m_sketches; }
        const BRepModel& profiles() const { return *m_profiles; }
        const FRepModel& objects() const { return *m_objects; }

        void print(std::ostream& os) const override;
    private:
        BRepModel* m_sketches;
        BRepModel* m_profiles;
        FRepModel* m_objects;

    };

};

