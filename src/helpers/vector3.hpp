#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <bullet3/LinearMath/btVector3.h>
#include "helpers/includes/raylib.hpp"
#include <iostream>

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

    inline Vector3f_t normalized () {
        glm::vec3 vector = glm::normalize(v);
        return Vector3f_t { vector.x, vector.y, vector.z };
    }


    // TODO: get direction vector
    inline Vector3f_t forward() {
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
    inline Vector3f_t right() {
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
    inline Vector3f_t up() {
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

    inline static void print(const Vector3f_t& vector) { 
        std::cout << glm::to_string(vector.v) << "\n";
    }

private:
    glm::vec3 v {};
};