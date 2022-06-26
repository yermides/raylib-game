#pragma once
#include "helpers/vectors.hpp"

enum class Projection_t {
        Perspective
    ,   Orthogonal
};

struct CCamera_t {
    Vector3f up     { 0.0f, 1.0f, 0.0f };
    Vector3f target { 0.0f, 0.0f, 0.0f };
    float fovy = 45.0f;
    Projection_t projection { Projection_t::Perspective };
};

struct CCameraRaylibData_t {
    
};
