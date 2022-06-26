#pragma once
#include "helpers/vectors.hpp"

struct CTransform_t {
    Vector3f position   { 0.0f, 0.0f, 0.0f };
    Vector3f rotation   { 0.0f, 0.0f, 0.0f };
    Vector3f scale      { 1.0f, 1.0f, 1.0f };
};
