#include "vector3.hpp"


// TODO: get direction vector
Vector3f_t Vector3f_t::forward() const {
    glm::vec3 front {};
    // yaw = y (left right), pitch = x (up down), roll = z (screw-like rotation)
    float yaw = v.y, pitch = v.x /*, roll = v.z */;

    front.x = -cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));

    front.x = glm::degrees(front.x);
    front.y = glm::degrees(front.y);
    front.z = glm::degrees(front.z);

    front = glm::normalize(front);
    return front;
}

// TODO:
Vector3f_t Vector3f_t::right() const {
    glm::vec3 result {};
    // float yaw = v.y;
    // {
    //     result.x = glm::cos(yaw);
    //     result.y = 0.0f;
    //     result.z = -glm::sin(yaw);            
    //     result = glm::normalize(result);
    // }

    {
        glm::vec3 dir = forward();
        result = glm::normalize(glm::cross(dir, glm::vec3{0,1,0}));
    }

    return result;
}

// TODO:
Vector3f_t Vector3f_t::up() const {
    glm::vec3 result {};

    // { // first option, I expect it to not work
    //     float yaw = v.y, pitch = v.x /*, roll = v.z */;

    //     result.x = glm::sin(pitch) * glm::sin(yaw);
    //     result.y = glm::cos(pitch);
    //     result.z = glm::sin(pitch) * glm::cos(yaw);
    // }
    {
        glm::vec3 r = right();
        glm::vec3 f = forward();
        result    = glm::normalize(glm::cross(r, f));
    }

    return result;
}

Vector3f_t Vector3f_t::normalized() const {
    glm::vec3 vector = glm::normalize(v);
    return Vector3f_t { vector.x, vector.y, vector.z };
}

Vector3f_t Vector3f_t::lerp(const Vector3f_t& v1, const Vector3f_t& v2, float percentage) {
    glm::vec3 v = glm::mix(v1.v, v2.v, percentage);
    return v;
}

std::string Vector3f_t::toString() const {
    return glm::to_string(v);
}
