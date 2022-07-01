#pragma once
#include "helpers/renderer.hpp"
#include "helpers/includes/raylib.hpp"

struct CModelRenderer_t : Renderer_t {
    RL::Model model;
};

// TODO: unused
struct CRaylibModelRenderer_t : Renderer_t, RL::Model {};

// CRaylibModelRenderer_t model {};