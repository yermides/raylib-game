#pragma once
#include "game/helpers/vector3.hpp"

// base class for shapes, should not be instantiated
struct CCollider_t {
    Vector3f_t offset {};
    void* runtimeCollisionShape {};
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

// TODO: composite collider & stop the user from using multiple collider components (shall use CCompoundCollider_t)

struct CCompoundCollider_t final : public CCollider_t {
    // TODO:
};
