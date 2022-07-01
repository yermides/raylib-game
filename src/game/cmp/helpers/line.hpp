#pragma once
#include "helpers/vector3.hpp"

struct DebugLine3D_t {
    explicit DebugLine3D_t(const Vector3f_t& p_from, const Vector3f_t& p_to) 
    : from(p_from), to(p_to)
    {}

    Vector3f_t from {}, to {};
};
