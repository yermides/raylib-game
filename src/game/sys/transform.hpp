#pragma once
#include "game/cmp/transform.hpp"

// struct Vector3f_t;
// struct CTransform_t;

Vector3f_t GetForwardVector(const CTransform_t& transform);
Vector3f_t GetRightVector(const CTransform_t& transform);
Vector3f_t GetUpVector(const CTransform_t& transform);
Vector3f_t GetBackVector(const CTransform_t& transform);
Vector3f_t GetLeftVector(const CTransform_t& transform);
Vector3f_t GetDownVector(const CTransform_t& transform);
void LookAt(CTransform_t& transform, const Vector3f_t& point);
