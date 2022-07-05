#include "transform.hpp"

Vector3f_t GetForwardVector(const CTransform_t& transform) {
    const float verticalAngle { transform.rotation.get_x() }, horizontalAngle { transform.rotation.get_y() };

    glm::vec3 forward = glm::vec3(
        glm::cos(glm::radians(verticalAngle)) * glm::sin(glm::radians(horizontalAngle))
    ,   glm::sin(glm::radians(verticalAngle))
    ,   glm::cos(glm::radians(verticalAngle)) * glm::cos(glm::radians(horizontalAngle))
    );

    forward = glm::normalize(glm::degrees(forward));

    return forward;
}

Vector3f_t GetRightVector(const CTransform_t& transform) {
    constexpr float kHalfPI = glm::half_pi<float>();
    const float horizontalAngle { transform.rotation.get_y() };

    glm::vec3 right = glm::vec3(
        glm::sin(glm::radians(horizontalAngle) - kHalfPI)
    ,   0
    ,   glm::cos(glm::radians(horizontalAngle) - kHalfPI)
    );

    right = glm::normalize(glm::degrees(right));

    return right;
}

Vector3f_t GetUpVector(const CTransform_t& transform) {
    glm::vec3 up = glm::cross(GetRightVector(transform).get(), GetForwardVector(transform).get());
    return up;
}

Vector3f_t GetBackVector(const CTransform_t& transform) {
    return -GetForwardVector(transform);
}

Vector3f_t GetLeftVector(const CTransform_t& transform) {
    return -GetRightVector(transform);
}

Vector3f_t GetDownVector(const CTransform_t& transform) {
    return -GetUpVector(transform);
}

void LookAt(CTransform_t& transform, const Vector3f_t& point) {
    // glm::lookAt(
    //         transform.position
    //     ,   (transform.position + GetForwardVector(transform).normalized())
    //     ,   GetUpVector(transform)
    // );
}
