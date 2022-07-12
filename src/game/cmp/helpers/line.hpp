#pragma once
#include "game/helpers/vector3.hpp"

struct DebugLine3D_t {
    explicit DebugLine3D_t(const Vector3f_t& p_from, const Vector3f_t& p_to) 
    : from(p_from), to(p_to)
    {}

    explicit DebugLine3D_t(const Vector3f_t& p_from, const Vector3f_t& p_to, const Vector3f_t& p_color) 
    : from(p_from), to(p_to), color(p_color)
    {}

    Vector3f_t from {}, to {}, color {}; // color is rgb, from 0 to 1
};
