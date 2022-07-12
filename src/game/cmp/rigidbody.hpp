#pragma once

// Rigidbodies cause/receive collisions as they are collision objects inside a world
// but their response as in-game objects are defined in another component

struct CCollider_t;

enum class BodyType_t {
        STATIC
    ,   KINEMATIC
    ,   DYNAMIC
};

// Single rigidbody. This object will only have one collision object with one type of filtering
struct CRigidbody_t {
    Vector3f_t angularFactor { 1.0f, 1.0f, 1.0f }; // Degree of freedom for each axis rotation (0 - 1)
    void* runtimeRigidbody {}; // maybe use std::any, we'll see
    CCollider_t* attachedCollider {};
    BodyType_t type { BodyType_t::STATIC };
    float mass { 0.0f };
};

// To use multiple colliders with different colliders, filtering flags and all for each one, I should use a multibody
// which, in essence, is a compound collider basically (bullet doesn't support more than one shape per body)
// Ok I was wrong, there is a Compound shape
// struct CRigidMultiBody {};
