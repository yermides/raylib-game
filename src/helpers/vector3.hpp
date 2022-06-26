#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "helpers/includes/raylib.hpp"
#include <iostream>

// Facade for glm::Vector3_t
struct Vector3_t {
    Vector3_t(float px = 0.0f, float py = 0.0f, float pz = 0.0f) {
        set(px, py, pz);
    }

    Vector3_t operator+(const Vector3_t& other) {
        return { 
                v.x + other.v.x
            ,   v.y + other.v.y
            ,   v.z + other.v.z
        };
    }

    Vector3_t& operator=(const Vector3_t& other) {
        v.x = other.v.x;
        v.y = other.v.y;
        v.z = other.v.z;
        return *this;
    }

    Vector3_t& operator+=(const Vector3_t& other) {
        Vector3_t result = (*this) + other;
        *this = result;
        return *this;
    }

    // implicit conversions with operators
    operator RL::Vector3 () { return { v.x, v.y, v.z }; }
    operator glm::vec3 () { return { v.x, v.y, v.z }; }

    constexpr const glm::vec3& get() const { return v; }
    constexpr const float& get_x() const { return v.x; }
    constexpr const float& get_y() const { return v.y; }
    constexpr const float& get_z() const { return v.z; }

    constexpr void set(glm::vec3 value) { v = value; }
    constexpr void set(float px, float py, float pz) { set_x(px); set_y(py); set_z(pz); }
    constexpr void set_x(float value) { v.x = value; }
    constexpr void set_y(float value) { v.y = value; }
    constexpr void set_z(float value) { v.z = value; }

    Vector3_t normalized () {
        glm::vec3 vector = glm::normalize(v);
        return Vector3_t { vector.x, vector.y, vector.z };
    }

    inline static void print(const Vector3_t& vector) { 
        std::cout << glm::to_string(vector.v) << "\n";
    }

private:
    glm::vec3 v {};
};