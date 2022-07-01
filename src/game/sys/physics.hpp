#pragma once
#include <memory>
#include "ecs/manager.hpp"
#include "helpers/vector3.hpp"
#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/btBulletDynamicsCommon.h>

// bullet3 physics system facade
struct SPhysics_t {
    explicit SPhysics_t(const Vector3f_t& gravity);
    ~SPhysics_t();

    void update(ECS::EntityManager_t& EntMan, const float deltatime);
private:
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDynamicsWorld> dynamicsWorld;
    std::unique_ptr<btIDebugDraw> debugDraw;
};
