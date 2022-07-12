#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <bullet3/LinearMath/btVector3.h>
#include "game/helpers/includes/raylib.hpp"

// Facade for glm::vec3
struct Vector3f_t {
    Vector3f_t(float px = 0.0f, float py = 0.0f, float pz = 0.0f) {
        set(px, py, pz);
    }

    Vector3f_t(const glm::vec3& pv) {
        set(pv);
    }

    Vector3f_t(const btVector3& pv) {
        set(pv.x(), pv.y(), pv.z());
    }

    Vector3f_t operator+(const Vector3f_t& other) {
        return { 
                v.x + other.v.x
            ,   v.y + other.v.y
            ,   v.z + other.v.z
        };
    }

    Vector3f_t operator-(const Vector3f_t& other) {
        return { 
                v.x - other.v.x
            ,   v.y - other.v.y
            ,   v.z - other.v.z
        };
    }

    Vector3f_t operator*(const float& other) {
        return { 
                v.x * other
            ,   v.y * other
            ,   v.z * other
        };
    }

    Vector3f_t& operator=(const Vector3f_t& other) {
        v.x = other.v.x;
        v.y = other.v.y;
        v.z = other.v.z;
        return *this;
    }

    Vector3f_t& operator+=(const Vector3f_t& other) {
        Vector3f_t result = (*this) + other;
        *this = result;
        return *this;
    }

    Vector3f_t& operator-=(const Vector3f_t& other) {
        Vector3f_t result = (*this) - other;
        *this = result;
        return *this;
    }

    Vector3f_t& operator*=(const float& other) {
        Vector3f_t result = (*this) * other;
        *this = result;
        return *this;
    }

    // implicit conversions with operators
    constexpr operator RL::Vector3()        { return { v.x, v.y, v.z }; }
    constexpr operator RL::Vector3() const  { return { v.x, v.y, v.z }; }
    constexpr operator glm::vec3()          { return { v.x, v.y, v.z }; }
    constexpr operator glm::vec3() const    { return { v.x, v.y, v.z }; }
    inline operator btVector3()             { return { v.x, v.y, v.z }; }
    inline operator btVector3() const       { return { v.x, v.y, v.z }; }

    constexpr const glm::vec3& get() const { return v; }
    constexpr const float& get_x() const { return v.x; }
    constexpr const float& get_y() const { return v.y; }
    constexpr const float& get_z() const { return v.z; }

    constexpr void set(const glm::vec3& value) { v = value; }
    constexpr void set(float px, float py, float pz) { set_x(px); set_y(py); set_z(pz); }
    constexpr void set_x(float value) { v.x = value; }
    constexpr void set_y(float value) { v.y = value; }
    constexpr void set_z(float value) { v.z = value; }

    constexpr void add_x(float value) { v.x += value; }
    constexpr void add_y(float value) { v.y += value; }
    constexpr void add_z(float value) { v.z += value; }

    Vector3f_t forward() const;
    Vector3f_t right() const;
    Vector3f_t up() const;

    Vector3f_t normalized() const;

    static Vector3f_t lerp(const Vector3f_t& v1, const Vector3f_t& v2, float percentage); // between 0 and 1

    inline friend std::ostream& operator<<(std::ostream& os, const Vector3f_t& vector) {
        os << vector.toString();
        return os;
    }

    std::string toString() const;
private:
    glm::vec3 v {};
};

// inline static void print(const Vector3f_t& vector) { 
//     std::cout << glm::to_string(vector.v) << "\n";
// }
