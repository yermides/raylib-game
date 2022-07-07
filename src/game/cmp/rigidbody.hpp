#pragma once

struct CCollider_t;

enum class BodyType_t {
        STATIC
    ,   KINEMATIC
    ,   DYNAMIC
};

struct CRigidbody_t {
    // degree of freedom for rotation (between 0 and 1). Set any axis to 0 to nullify rot (constrain the entire axis)
    Vector3f_t angularFactor { 1.0f, 1.0f, 1.0f };
    void* runtimeBullet3Body {}; // maybe use std::any, we'll see
    CCollider_t* attachedCollider {};
    BodyType_t type { BodyType_t::STATIC };
    float mass { 0.0f };
};
