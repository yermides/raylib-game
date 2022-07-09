#include "physics_commands.hpp"
#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet3/btBulletDynamicsCommon.h>
#include "game/cmp/helpers/all.hpp"
#include <tuple>

void PhysicsCommandProcessor_t::update(ECS::EntityManager_t& EntMan) {
    while(!commands.empty()) {
        commands.front()->execute(EntMan, *this, m_Context);
        commands.pop();
    }
}

/*
bool HasAnyCollisionComponents(ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
    return EntMan.hasAnyComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(e);
}

btCollisionShape* GetCollisionShapeFromAnyComponent(ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
    btCollisionShape* collisionShape { nullptr };

    if (EntMan.hasAnyComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(e)) {
        auto [box, sphere, capsule] = EntMan.tryGetComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(e);
        
        CCollider_t* base {};
        
        if(box)     base = box;
        if(sphere)  base = sphere;
        if(capsule) base = capsule;

        collisionShape = static_cast<btCollisionShape*>(base->runtimeBullet3CollisionShape);
    }

    return collisionShape;
}
*/

btCollisionShape* TryObtainCollisionShape(ECS::Entityid_t target, ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) {
    btCollisionShape* collisionShape {};

    // Try to get collision shape of out collision components, if not, shape will remain null
    if (EntMan.hasAnyComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(target)) {
        auto [box, sphere, capsule] = EntMan.tryGetComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(target);
        
        CCollider_t* base {};
        
        if(box)     base = box;
        if(sphere)  base = sphere;
        if(capsule) base = capsule;

        collisionShape = static_cast<btCollisionShape*>(base->runtimeBullet3CollisionShape);

        // If we have a component that will define a shape, configure the shape in-place and get it
        if(!collisionShape) {
            std::unique_ptr<IPhysicsCommand_t> command = std::make_unique<AddColliderToWorldCommand_t>(target);
            command->execute(EntMan, processor, context);
            collisionShape = static_cast<btCollisionShape*>(base->runtimeBullet3CollisionShape);
        }
    }

    return collisionShape;
}

btTransform TransferEntityTransform(CTransform_t& transformComponent) {
    // Configure starting position and rotation same as the entities'
    const Vector3f_t& startingBodyTranslation { transformComponent.position }
    , startingBodyRotation { transformComponent.rotation };

    btTransform bulletTransform;
    btQuaternion bulletOrientation;
    bulletOrientation.setEuler(startingBodyRotation.get_y(), startingBodyRotation.get_x(), startingBodyRotation.get_z());
    bulletTransform.setIdentity();
    bulletTransform.setOrigin(startingBodyTranslation);
    bulletTransform.setRotation(bulletOrientation);

    return bulletTransform;
}

std::tuple<float, btVector3> CalculateMassAndInertia(CRigidbody_t& rigidbodyComponent, btCollisionShape* collisionShape) {
    // Set the type of rigidbody and it's mass and inertia
    btVector3 localInertia { 0.0f, 0.0f, 0.0f };
    btScalar mass { 0.0f };

    if(rigidbodyComponent.type == BodyType_t::DYNAMIC) {
        mass = std::max(1.0f, rigidbodyComponent.mass);

        if(collisionShape) {
            collisionShape->calculateLocalInertia(mass, localInertia);
        }
    }

    rigidbodyComponent.mass = mass;

    return { mass, localInertia };
}

void SetCollisionFlags(const CRigidbody_t& rigidbodyComponent, btRigidBody* bulletRigidbody) {
    // Set bullet's internal collision flags that discriminate collision contacts interaction
    if(!bulletRigidbody) return;

    const int currentFlags = bulletRigidbody->getCollisionFlags();

    switch (rigidbodyComponent.type) {
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
    // collisionShape->setUserPointer(colliderComponent); // not really needed for now but may be useful

    // TODO: need to improve the ability to add colliders at runtime

    // if(CRigidbody_t* rigidbody = EntMan.tryGetComponent<CRigidbody_t>(m_Target)) {

    //     if(!rigidbody->attachedCollider) {
    //         rigidbody->attachedCollider = colliderComponent;

    //     }

    //     // Maybe I should add a command that would add a collider to a collision object, but this is ok
    //     if(rigidbody->runtimeBullet3Body) {
    //         btRigidBody* bulletRigidbody = static_cast<btRigidBody*>(rigidbody->runtimeBullet3Body);
    //         bulletRigidbody->setCollisionShape(collisionShape);
    //         // maybe also calculate local inertia
    //     } 
    //     // else {
    //     //     std::unique_ptr<IPhysicsCommand_t> command = std::make_unique<AddRigidbodyToWorldCommand_t>(m_Target);
    //     //     command->execute(EntMan, processor, context);
    //     // }
    // }

}

void AddRigidbodyToWorldCommand_t::execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) {
    // Stop the command if it doesn't have rigidbody or it has already been configured, runtimeBody won't be null
    if(
        !EntMan.hasComponent<CRigidbody_t>(m_Target) 
        || EntMan.getComponent<CRigidbody_t>(m_Target).runtimeBullet3Body
    ) return;

    btCollisionShape* collisionShape = TryObtainCollisionShape(m_Target, EntMan, processor, context);

    // Start configuring the rigidbody from the component data
    auto [transform, rigidbody] = EntMan.getComponents<CTransform_t, CRigidbody_t>(m_Target);
    btTransform bulletTransform = TransferEntityTransform(transform);
    auto [mass, localInertia] = CalculateMassAndInertia(rigidbody, collisionShape);
    
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

    // Rigidbody axis rotation constraints
    bulletRigidbody->setAngularFactor(rigidbody.angularFactor);
    SetCollisionFlags(rigidbody, bulletRigidbody);
}

void AddTriggerToWorldCommand_t::execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) {
    if(
        !EntMan.hasComponent<CTriggerVolume_t>(m_Target) 
        || EntMan.getComponent<CTriggerVolume_t>(m_Target).runtimeTriggerObject
    ) return;

    auto [transform, trigger] = EntMan.getComponents<CTransform_t, CTriggerVolume_t>(m_Target);

    btCollisionShape* collisionShape = TryObtainCollisionShape(m_Target, EntMan, processor, context);
    btGhostObject* ghostObject = new btPairCachingGhostObject(); // maybe use btPairCachingGhostObject*
    btTransform bulletTransform = TransferEntityTransform(transform);

    if(collisionShape)
        ghostObject->setCollisionShape(collisionShape);
    
    ghostObject->setWorldTransform(bulletTransform);

    if(context.dynamicsWorld) {
        context.dynamicsWorld->addCollisionObject(ghostObject);
        ghostObject->setUserPointer(new TriggerUserPointer_t{&EntMan, &trigger});
        trigger.runtimeTriggerObject = ghostObject;
    }

    ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    // Save it for checks inside InternalTickCallback of SPhysics_t, now I need to remove it if Trigger component gets removed
    if(context.triggers) {
        auto e = EntMan.getEntity(trigger);
        context.triggers->push_back(e);
    }
}

void AddCharacterToWorldCommand_t::execute(ECS::EntityManager_t& EntMan, PhysicsCommandProcessor_t& processor, PhysicsContext_t& context) {
    // TODO:
}
