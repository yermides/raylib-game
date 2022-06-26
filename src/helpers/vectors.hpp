#pragma once
#include <glm/glm.hpp>

using Vector3f = glm::vec3;

/*
#include "helpers/includes/raylib.hpp"
struct Vector3 {
    explicit Vector3(float px, float py, float pz);

    float x() {
        return v.x;
    }

    operator RL::Vector3 () { 
        return { v.x, v.y, v.z };
    }

    operator glm::vec3 () {
        return { v.x, v.y, v.z };
    }

    Vector3 normalized () {
        glm::vec3 vector = glm::normalize(v);
        return Vector3 { vector.x, vector.y, vector.z };
    }

private:
    glm::vec3 v {};
};
*/