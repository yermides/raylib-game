#pragma once
// Facade for glm::vec2
#include <glm/glm.hpp>
#include "helpers/includes/raylib.hpp"

struct Vector2f_t {
    Vector2f_t(const glm::vec2& pv) {
        set(pv);
    }

    Vector2f_t(float px = 0.0f, float py = 0.0f) {
        set(px, py);
    }

    Vector2f_t operator+(const Vector2f_t& other) {
        return { 
                v.x + other.v.x
            ,   v.y + other.v.y
        };
    }

    Vector2f_t operator-(const Vector2f_t& other) {
        return { 
                v.x - other.v.x
            ,   v.y - other.v.y
        };
    }

    Vector2f_t& operator=(const Vector2f_t& other) {
        v.x = other.v.x;
        v.y = other.v.y;
        return *this;
    }

    Vector2f_t& operator+=(const Vector2f_t& other) {
        Vector2f_t result = (*this) + other;
        *this = result;
        return *this;
    }

    Vector2f_t& operator-=(const Vector2f_t& other) {
        Vector2f_t result = (*this) - other;
        *this = result;
        return *this;
    }

    // implicit conversions with operators
    operator RL::Vector2 () { return { v.x, v.y }; }
    operator glm::vec2 () { return { v.x, v.y }; }

    constexpr const glm::vec2& get() const { return v; }
    constexpr const float& get_x() const { return v.x; }
    constexpr const float& get_y() const { return v.y; }

    constexpr void set(const glm::vec2& value) { v = value; }
    constexpr void set(float px, float py) { set_x(px); set_y(py); }
    constexpr void set_x(float value) { v.x = value; }
    constexpr void set_y(float value) { v.y = value; }

private:
    glm::vec2 v {};
};
