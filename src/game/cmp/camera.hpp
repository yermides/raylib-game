#pragma once
#include "helpers/vectors.hpp"
#include "helpers/vector3.hpp"

enum class Projection_t {
        Perspective
    ,   Orthogonal
};

struct CCamera_t {
    Vector3f_t up     { 0.0f, 1.0f, 0.0f };
    Vector3f_t target { 0.0f, 0.0f, 0.0f };
    float fovy = 45.0f;
    Projection_t projection { Projection_t::Perspective };
};

struct CCameraRaylibData_t {
    
};
