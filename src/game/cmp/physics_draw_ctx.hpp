#pragma once 
#include <vector>
#include "helpers/line.hpp"

// Singleton Component

struct SCPhysicsDrawingContext_t {
    const std::vector<DebugLine3D_t>* lines {};
};
