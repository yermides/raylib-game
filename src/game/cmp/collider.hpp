#pragma once
#include "helpers/vector3.hpp"

// base class, should not be instantiated
struct CCollider_t {
    Vector3f_t offset {};
    void* runtimeBullet3CollisionShape {};
};

struct CBoxCollider_t final : public CCollider_t {
    Vector3f_t boxHalfExtents { 0.5f, 0.5f, 0.5f };
};

struct CSphereCollider_t final : public CCollider_t {
    float radius { 0.5f };
};

struct CCapsuleCollider_t final : public CCollider_t {
    float height { 2.0f }, radius { 0.5f };
};
