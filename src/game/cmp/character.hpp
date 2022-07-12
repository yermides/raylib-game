#pragma once
#include <queue>
#include "game/helpers/vector3.hpp"

// TODO: implement in the physics system part
struct CCharacterController_t {
    std::queue<Vector3f_t> impulses {};
    void* runtimeCharacterController {};
};
