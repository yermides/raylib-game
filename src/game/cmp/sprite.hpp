#pragma once
#include "helpers/renderer.hpp"
#include "helpers/includes/raylib.hpp"

enum class SpriteRenderMode_t {
        TWO_DIMENSIONS
    ,   THREE_DIMENSIONS
};

struct SpriteRenderer_t {
    SpriteRenderMode_t mode { SpriteRenderMode_t::TWO_DIMENSIONS };
    
    /* data */
};