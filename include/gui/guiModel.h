#pragma once
#include "document/model.h"

/**
 * GuiModel represents the data model for the GUI.
 * It will contain the display list of graphical objects to be rendered, and potentially other GUI-related data in future.  
 */

namespace e2 {
    class GuiModel : public Model {
    public:
        GuiModel() : Model() {}
        void print(std::ostream& os) const override {
            os << "GuiModel" << std::endl;
        }

        // Empty for now. Will add GUI-specific data members and methods later.
    private:
    };
};

