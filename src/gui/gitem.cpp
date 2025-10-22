#include "gui/gitem.h"

namespace e2 {

    std::ostream& operator<<(std::ostream& os, const GItem& m){  
        m.print(os);
        return os;
    }
    
    void GBlock::print(std::ostream& os) const {
        os << "GBlock" << "(length = " << m_length << ", width=" << m_width << ", height=" << m_height << ")";
    }

};

