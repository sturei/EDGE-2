#include "gui/gitem.h"

namespace e2 {

    std::ostream& operator<<(std::ostream& os, const GItem& m){  
        m.print(os);
        return os;
    }

    void GPoint::print(std::ostream& os) const {
        os << "GPoint" << "(size=" << m_size << ")";
    }   
    
    void GLine::print(std::ostream& os) const {
        os << "GLine" << "(length=" << m_length << ")";
    }

    void GPlane::print(std::ostream& os) const {
        os << "GPlane" << "(width=" << m_width << ", height=" << m_height << ")";
    }

    void GSphere::print(std::ostream& os) const {
        os << "GSphere" << "(radius=" << m_radius << ")";
    }

    void GBlock::print(std::ostream& os) const {
        os << "GBlock" << "(width=" << m_width << ", height=" << m_height << ", depth=" << m_depth << ")";
    }

};

