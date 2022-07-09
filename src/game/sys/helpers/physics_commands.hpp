#pragma once
#include <queue>
#include <memory>
#include "ecs/manager.hpp"

class btDynamicsWorld;
struct SPhysics_t;
struct IPhysicsCommand_t;
struct PhysicsCommandProcessor_t;
struct CRigidbody_t;
struct CTriggerVolume_t;

struct RigidbodyUserPointer_t {
    ECS::EntityManager_t* entityManager {};
    CRigidbody_t* rigidbodyComponent    {};
};

struct TriggerUserPointer_t {
    ECS::EntityManager_t* entityManager {};
    CTriggerVolume_t* triggerComponent  {};
};

struct PhysicsContext_t {
    btDynamicsWorld* dynamicsWorld {};
    std::vector<ECS::Entityid_t>* triggers {};
};

struct IPhysicsCommand_t {
    virtual void execute(ECS::EntityManager_t&, PhysicsCommandProcessor_t&, PhysicsContext_t&) = 0;
    virtual ~IPhysicsCommand_t() = default;
};

struct PhysicsCommandProcessor_t {
    PhysicsCommandProcessor_t(PhysicsContext_t& context) : m_Context(context) {}

    template <typename Command, typename... Args>
    void add(Args&&... args) {
        commands.push(std::make_unique<Command>(std::forward<Args>(args)...));
    }

    void update(ECS::EntityManager_t& EntMan);
private:
    using PhysicsCommandQueue_t = std::queue<std::unique_ptr<IPhysicsCommand_t>>;
    PhysicsContext_t& m_Context;
    PhysicsCommandQueue_t commands {};
    // SPhysics_t& Physics; // maybe
};


struct AddColliderToWorldCommand_t final : public IPhysicsCommand_t {
    explicit AddColliderToWorldCommand_t(ECS::Entityid_t e) : m_Target(e) {}
    void execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) override;
private:
    ECS::Entityid_t m_Target {};
};

struct AddRigidbodyToWorldCommand_t final : public IPhysicsCommand_t {
    explicit AddRigidbodyToWorldCommand_t(ECS::Entityid_t e) : m_Target(e) {}
    void execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) override;
private:
    ECS::Entityid_t m_Target {};
};

struct AddTriggerToWorldCommand_t final : public IPhysicsCommand_t {
    explicit AddTriggerToWorldCommand_t(ECS::Entityid_t e) : m_Target(e) {}
    void execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) override;
private:
    ECS::Entityid_t m_Target {};
};

struct AddCharacterToWorldCommand_t final : public IPhysicsCommand_t {
    explicit AddCharacterToWorldCommand_t(ECS::Entityid_t e) : m_Target(e) {}
    void execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) override;
private:
    ECS::Entityid_t m_Target {};
};

