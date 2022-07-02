#pragma once
#include <memory>
#include <vector>
#include "ecs/manager.hpp"
#include "helpers/vector3.hpp"
#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/btBulletDynamicsCommon.h>

// bullet3 physics system facade, units are meters, just like raylib
struct SPhysics_t {
    explicit SPhysics_t(const Vector3f_t& gravity);
    ~SPhysics_t();

    void update(ECS::EntityManager_t& EntMan, const float deltatime);

    // ECS Events
    void registerAddToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e);
private:
    void addEntitiesToWorld(ECS::EntityManager_t& EntMan);

    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDynamicsWorld> dynamicsWorld;
    std::unique_ptr<btIDebugDraw> debugDraw;

    std::vector<ECS::Entityid_t> entitiesToAddWorld {};
};
