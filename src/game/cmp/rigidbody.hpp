#pragma once

enum class BodyType_t {
        STATIC
    ,   KINEMATIC
    ,   DYNAMIC
};

struct CRigidbody_t {
    BodyType_t type { BodyType_t::STATIC };
    float mass { 0.0f };
    void* runtimeBullet3Body {}; // maybe use std::any, we'll see
};
