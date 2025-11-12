#pragma once

#include "frep/fobject.h"

namespace e2 {
    
    size_t arity(const FObject& object, FNodeIndex nodeIndex);
    std::vector<FNodeIndex> getInputFNodes(const FObject& object, FNodeIndex nodeIndex);

};