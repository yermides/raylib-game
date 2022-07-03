#pragma once
#include <queue>
#include "helpers/vector3.hpp"

// TODO: implement in the physics system part
struct CCharacterController_t {
    std::queue<Vector3f_t> impulses {};
    void* runtimeCharacterController {};
};
