#pragma once
#include "helpers/renderer.hpp"
// #include "helpers/fwd.hpp"
#include "helpers/includes/raylib.hpp"

struct CModelRenderer_t : Renderer_t {
    RL::Model model;
};
