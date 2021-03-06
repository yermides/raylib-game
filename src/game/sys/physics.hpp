#pragma once
#include <memory>
#include <vector>
#include "ecs/manager.hpp"
#include "helpers/physics_commands.hpp"
#include <unordered_map>
// #include "game/helpers/types.hpp"

struct Vector3f_t;
struct CRigidbody_t;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDynamicsWorld;
class btIDebugDraw;
class btPersistentManifold;
struct RigidbodyUserPointer_t;

// bullet3 physics system facade, units are meters, just like raylib
struct SPhysics_t {
    explicit SPhysics_t(const Vector3f_t& gravity);
    ~SPhysics_t();

    void setGravity(const Vector3f_t& gravity);

    void update(ECS::EntityManager_t& EntMan, const float deltatime);

    // Vector3f_t raycast() // just to not forget

    // ECS Events, TODO: use some kind of command pool or something, it would clean the code
    void registerAddColliderToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e); // collision shapes really
    void registerAddRigidbodyToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e);
    void registerAddTriggerToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e);
    void registerAddCharacterToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e);
    void removeAndDeleteBodyFromWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e);
private:
    void initPhysicsContext();
    void uploadDebugDrawContext(ECS::EntityManager_t& EntMan);
    static void bulletInternalTickCallback(btDynamicsWorld* world, float timeStep);
    static void bulletContactStartedCallback(btPersistentManifold* const& manifold);
    static void bulletContactEndedCallback(btPersistentManifold* const& manifold);

    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDynamicsWorld> dynamicsWorld;
    std::unique_ptr<btIDebugDraw> debugDraw;

    // I should store the trigger entities because their pairs are not checked by the world but by themselves
    // std::vector<CTriggerVolume_t> triggers {} or something

    PhysicsContext_t m_PhysicsContext {};
    PhysicsCommandProcessor_t commandProcessor { m_PhysicsContext };
    std::vector<ECS::Entityid_t> m_CachedTriggerObjects {};

    // using PhysicsTypeCallback = std::function<void(CRigidbody_t&, CRigidbody_t&)>;
    // inline static std::unordered_map<int, PhysicsTypeCallback>
};

void ApplyForce(CRigidbody_t& rigidbody, const Vector3f_t& force, const Vector3f_t& relativePosition);
void ApplyCentralForce(CRigidbody_t& rigidbody, const Vector3f_t& force);
void ApplyTorque(CRigidbody_t& rigidbody, const Vector3f_t& torque);
void ApplyImpulse(CRigidbody_t& rigidbody, const Vector3f_t& impulse, const Vector3f_t& relativePosition);
void ApplyCentralImpulse(CRigidbody_t& rigidbody, const Vector3f_t& force);
void SetLinearVelocity(CRigidbody_t& rigidbody, const Vector3f_t& velocity);
void SetAngularVelocity(CRigidbody_t& rigidbody, const Vector3f_t& velocity);
void Translate(CRigidbody_t& rigidbody, const Vector3f_t& movement);
void ClearForces(CRigidbody_t& rigidbody);

/*
    Rigidbody rules:
        - Only dynamic objects can be applied forces
        - An object is automatically dynamic if it has mass (that's how bullet does it)
        - That means both static and kinematics are unaffected by gravity
        - To move a kinematic object, use the translate function
        - Contacts only occur if a dynamic object touches other object (no matter the other's type)
        - If I want to set custom contact addition, I need to use my own Broadphase Filter Callback (bullet docs p17)
*/

/*
    Important information regarding bullet's substepping:
    If I specify a maxSubSteps value greater than zero, it expects the timestep to be constant (which it should)
    Also, substepping is called whenever the deltatime exceds fixedDelta (the excess is accumulated internally)
    So, for example, a 165hz refresh of the main thread calling for a 50hz physics will cause ~3 internal substeps
    So maxSubSteps should be 4 to be in range for a fully capable interpolation (?)

    It goes both ways in the sense that, if the delta is big (slow framerate from main thread)
    it will subdivide that delta and perform the same step-by-step comprobation to get an accurate result

    Also, it's not a bad idea to check if I should copy translation & rotation from physics during internalTickCallback
    Although this doesn't seem to be an issue UNTIL I use an specific physics thread running at 50 ~ 60 fps
    Basically 'cause right now they're being updated at 60fps so the movement transfer is identical 
    And, casually, there is no interpolation required (thoght activated for mentioned reasons)
    It also goes both ways: if the physics would be updated slowly it would take more time to produce a substep

    More sidenotes:
    I dunno what would be more efficient and best for interpolation:
    dynamicsWorld->stepSimulation(deltatime: 1/165, maxSubSteps, kFixedTimestep: 1/60);
    or
    dynamicsWorld->stepSimulation(deltatime: 1/60, maxSubSteps, kFixedTimestep: 1/165);

    -- TODO: read this --
    I guess I'll have to test it out
    Also, brief note that the motionState could play a role
    Yes, it surely does: to make a good interpolation I'd need a motionState derived class
    and in the setWorldTransform() update the entities' transform
    Why? because the setWorldTransform() is the method called during the interpolation phase
    so the correct answer to the previous is (including the use of a user-defined motionState derived class)
    dynamicsWorld->stepSimulation(deltatime: 1/165 (the render thread update speed), maxSubSteps, kFixedTimestep: 1/60);
*/
