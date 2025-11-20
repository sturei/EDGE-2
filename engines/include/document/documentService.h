#pragma once
#include "document/document.h"

namespace e2 {
    namespace DocumentService {
        bool runOnce(Document* document, std::istream& input = std::cin, std::ostream& output = std::cout);
        void run(Document* document, std::istream& input = std::cin, std::ostream& output = std::cout);
    }
};  