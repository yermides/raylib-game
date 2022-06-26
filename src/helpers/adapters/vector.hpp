#pragma once
#include "helpers/vectors.hpp"
#include "helpers/includes/raylib.hpp"

namespace VectorAdapter_t {
    RL::Vector3 adapt(const Vector3f&);
    Vector3f adapt(const RL::Vector3&);
}

namespace V3A = VectorAdapter_t;
