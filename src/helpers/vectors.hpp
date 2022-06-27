// #pragma once
// #include <glm/glm.hpp>

// using Vector3f = glm::vec3;

// #include "helpers/includes/raylib.hpp"

// Facade for glm::vec3
// struct Vector3 {
//     Vector3(float px = 0.0f, float py = 0.0f, float pz = 0.0f) {
//         set(px, py, pz);
//     }

//     constexpr const glm::vec3& get() const { return v; }
//     constexpr const float& get_x() const { return v.x; }
//     constexpr const float& get_y() const { return v.y; }
//     constexpr const float& get_z() const { return v.z; }

//     constexpr void set(const glm::vec3& value) { v = value; }
//     constexpr void set(float px, float py, float pz) { set_x(px); set_y(py); set_z(pz); }
//     constexpr void set_x(float value) { v.x = value; }
//     constexpr void set_y(float value) { v.y = value; }
//     constexpr void set_z(float value) { v.z = value; }

//     operator RL::Vector3 () { 
//         return { v.x, v.y, v.z };
//     }

//     operator glm::vec3 () {
//         return { v.x, v.y, v.z };
//     }

//     Vector3 normalized () {
//         glm::vec3 vector = glm::normalize(v);
//         return Vector3 { vector.x, vector.y, vector.z };
//     }

// private:
//     glm::vec3 v {};
// };

// using Vector3f = Vector3;