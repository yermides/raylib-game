#include "physics_commands.hpp"
#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet3/btBulletDynamicsCommon.h>
#include "game/cmp/helpers/all.hpp"

void PhysicsCommandProcessor_t::update(ECS::EntityManager_t& EntMan) {
    while(!commands.empty()) {
        commands.front()->execute(EntMan, *this, m_Context);
        commands.pop();
    }
}

void AddColliderToWorldCommand_t::execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) {
    if(!EntMan.hasAnyComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(m_Target)) return;

    CCollider_t*      colliderComponent {};
    btCollisionShape* collisionShape    {};

    if(EntMan.hasComponent<CBoxCollider_t>(m_Target)) {
        CBoxCollider_t& collider = EntMan.getComponent<CBoxCollider_t>(m_Target);
        collisionShape = new btBoxShape(collider.boxHalfExtents);
        colliderComponent = &collider;
    } else if (EntMan.hasComponent<CSphereCollider_t>(m_Target)) {
        CSphereCollider_t& collider = EntMan.getComponent<CSphereCollider_t>(m_Target);
        collisionShape = new btSphereShape(collider.radius);
        colliderComponent = &collider;
    } else if (EntMan.hasComponent<CCapsuleCollider_t>(m_Target)) {
        CCapsuleCollider_t& collider = EntMan.getComponent<CCapsuleCollider_t>(m_Target);
        collisionShape = new btCapsuleShape(collider.radius, collider.height);
        colliderComponent = &collider;
    }

    colliderComponent->runtimeBullet3CollisionShape = collisionShape;

    // if(CRigidbody_t* rigidbody = EntMan.tryGetComponent<CRigidbody_t>(m_Target)) {
    //     if(rigidbody->runtimeBullet3Body) {
    //         // try adding rigidbody again, depends on the implementation of the other commands
    //     } else {

    //     }
    // }

}

void AddRigidbodyToWorldCommand_t::execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) {
    // Stop the command if it doesn't have rigidbody or it has already been configured, runtimeBody won't be null
    if(
        !EntMan.hasComponent<CRigidbody_t>(m_Target) 
        || EntMan.getComponent<CRigidbody_t>(m_Target).runtimeBullet3Body
    ) return;

    btCollisionShape* collisionShape {};

    // Try to get collision shape of out collision components, if not, shape will remain null
    if (EntMan.hasAnyComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(m_Target)) {
        auto [box, sphere, capsule] = EntMan.tryGetComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(m_Target);
        
        CCollider_t* base {};
        
        if(box)     base = box;
        if(sphere)  base = sphere;
        if(capsule) base = capsule;

        collisionShape = static_cast<btCollisionShape*>(base->runtimeBullet3CollisionShape);

        // If we have a component that will define a shape, configure the shape in-place and get it
        if(!collisionShape) {
            std::unique_ptr<IPhysicsCommand_t> command = std::make_unique<AddColliderToWorldCommand_t>(m_Target);
            command->execute(EntMan, processor, context);
            collisionShape = static_cast<btCollisionShape*>(base->runtimeBullet3CollisionShape);
        }
    }

    // Start configuring the rigidbody from the component data
    auto [transform, rigidbody] = EntMan.getComponents<CTransform_t, CRigidbody_t>(m_Target);

    // Configure starting position and rotation same as the entities'
    const Vector3f_t& startingBodyTranslation { transform.position }, startingBodyRotation { transform.rotation };
    
    btTransform bulletTransform;
    btQuaternion bulletOrientation;
    bulletOrientation.setEuler(startingBodyRotation.get_y(), startingBodyRotation.get_x(), startingBodyRotation.get_z());
    bulletTransform.setIdentity();
    bulletTransform.setOrigin(startingBodyTranslation);
    bulletTransform.setRotation(bulletOrientation);

    // Set the type of rigidbody and it's mass and inertia
    btVector3 localInertia { 0.0f, 0.0f, 0.0f };
    btScalar mass { 0.0f };

    if(rigidbody.type == BodyType_t::DYNAMIC) {
        mass = std::max(1.0f, rigidbody.mass);

        if(collisionShape) {
            collisionShape->calculateLocalInertia(mass, localInertia);
        }
    }

    rigidbody.mass = mass;

    // Set internal motion state, useful for internal interpolation during step calls
    btDefaultMotionState* myMotionState = new btDefaultMotionState(bulletTransform);

    // Final construction with all the information
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
    btRigidBody* bulletRigidbody = new btRigidBody(rbInfo);

    // Add rigidbody to world, applies it's gravity
    if(context.dynamicsWorld) {
        context.dynamicsWorld->addRigidBody(bulletRigidbody); // also allows filter groups and masks
        // To reset filter group, you need to get it from the current hande
        // bulletRigidbody->getBroadphaseHandle()->m_collisionFilterGroup = ...
        // That kinda indicates it shouldn't be touched once setup

        // Set user pointers, both in the component and in the bullet's body
        rigidbody.runtimeBullet3Body = bulletRigidbody;
        bulletRigidbody->setUserPointer(new RigidbodyUserPointer_t { &EntMan, &rigidbody });
    }

    // Additional rigidbody configuration

    // Rigidbody axis rotation constraints
    bulletRigidbody->setAngularFactor(rigidbody.angularFactor);

    // Set bullet's internal collision flags that discriminate collision contacts interaction
    const int currentFlags = bulletRigidbody->getCollisionFlags();

    switch (rigidbody.type) {
        using btCO = btCollisionObject;

        case BodyType_t::STATIC:
            bulletRigidbody->setCollisionFlags(currentFlags | btCO::CF_STATIC_OBJECT);
        break;
        case BodyType_t::KINEMATIC:
	        // body->setActivationState(DISABLE_DEACTIVATION); // May be needed for the motion state
	        bulletRigidbody->setCollisionFlags(currentFlags | btCO::CF_KINEMATIC_OBJECT);
        break;
        case BodyType_t::DYNAMIC:
	        bulletRigidbody->setCollisionFlags(currentFlags | btCO::CF_DYNAMIC_OBJECT);
        break;
    }
}

void AddTriggerToWorldCommand_t::execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) {
    // TODO:
}

void AddCharacterToWorldCommand_t::execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) {
    // TODO:
}
