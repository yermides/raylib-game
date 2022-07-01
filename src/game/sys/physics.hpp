#pragma once
#include "ecs/manager.hpp"
#include "helpers/vector3.hpp"

// bullet3 physics system facade
struct SPhysics_t {
    explicit SPhysics_t(const Vector3f_t& gravity);
    void update(ECS::EntityManager_t& EntMan);

private:
    /* data */
};
