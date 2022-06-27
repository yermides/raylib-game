#pragma once
#include "helpers/vectors.hpp"
#include "helpers/vector3.hpp"

struct CTransform_t {
    Vector3f_t position   { 0.0f, 0.0f, 0.0f };
    Vector3f_t rotation   { 0.0f, 0.0f, 0.0f };
    Vector3f_t scale      { 1.0f, 1.0f, 1.0f };
};
