#include "physics.hpp"
#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet3/btBulletDynamicsCommon.h>
#include "game/helpers/vector3.hpp"
#include "game/cmp/helpers/all.hpp"
#include "helpers/physics_drawer.hpp"
#include "game/helpers/logger.hpp"

#include <iostream>

btRigidBody& GetBullet3Rigidbody(const CRigidbody_t& rigidbody); // declaration

SPhysics_t::SPhysics_t(const Vector3f_t& gravity) {
    collisionConfiguration  = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher              = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
    overlappingPairCache    = std::make_unique<btDbvtBroadphase>();
    solver                  = std::make_unique<btSequentialImpulseConstraintSolver>();

    dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        dispatcher.get()
    ,   overlappingPairCache.get()
    ,   solver.get()
    ,   collisionConfiguration.get()
    );

    debugDraw = std::make_unique<Bullet3PhysicsDrawer_t>();
    dynamicsWorld->setDebugDrawer(debugDraw.get());
    dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

    setGravity(gravity);

    gContactStartedCallback = bulletContactStartedCallback;
    gContactEndedCallback = bulletContactEndedCallback;
    dynamicsWorld->setInternalTickCallback(bulletInternalTickCallback, this, true);

    initPhysicsContext();
}

SPhysics_t::~SPhysics_t() {
    // TODO: maybe needs to be improved, but it's enough for now
    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

        if(btCollisionShape* shape = obj->getCollisionShape(); shape) {
            collisionShapes.push_back(shape);
        }

		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}

		dynamicsWorld->removeCollisionObject(obj);

        // FIXME: i'm assuming a rigidbody here, but it could be a collisionObject or ghostObject
        // right now if I add those it could crash (or no because of the null check from below)
        RigidbodyUserPointer_t* userPointer = static_cast<RigidbodyUserPointer_t*>(obj->getUserPointer());

        if(userPointer) {
            delete userPointer;
            LOG_CORE_INFO("User pointer freed from rigidbody");
        }

		delete obj;
	}

	//delete collision shapes
	for (int j = collisionShapes.size() - 1; j >= 0; j--) {
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = nullptr;
		delete shape;
	}
}

void SPhysics_t::setGravity(const Vector3f_t& gravity) {
    dynamicsWorld->setGravity(gravity);
}

void SPhysics_t::update(ECS::EntityManager_t& EntMan, const float deltatime) {
    auto updateEntitiesTransform = [](auto e, CTransform_t& transform, CRigidbody_t& rigidbody) {
        // Update transform with rigidbody's transform
        btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);

        // copy translation, TODO: use offset
        transform.position = bullet3body.getWorldTransform().getOrigin();

        // copy rotation
        float x {}, y {}, z {};
        btQuaternion orientation = bullet3body.getWorldTransform().getRotation();
        orientation.getEulerZYX(z, y, x);
        x = btDegrees(x);
        y = btDegrees(y);
        z = btDegrees(z);

        transform.rotation.set(x, y, z);
    };

    // Check contacts between objects without contact response (triggers, ghost objects essentially)
    auto checkTriggerCollisions = [this](auto e, CTriggerVolume_t& trigger) {
        btPairCachingGhostObject* ghostObject = static_cast<btPairCachingGhostObject*>(trigger.runtimeTriggerObject);

        if(!ghostObject) return;

        btManifoldArray manifoldArray;
        btBroadphasePairArray& pairs = ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
        int numPairs = pairs.size();

        for(int i {}; i < numPairs; ++i) {
            manifoldArray.clear();
            const btBroadphasePair &pair = pairs[i];

            btBroadphasePair *collisionPair = dynamicsWorld->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
            
            if(!collisionPair) return;

            if(collisionPair->m_algorithm) {
                collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);
            }

            // std::cout << manifoldArray.size() << "\n";
            // LOG_CORE_INFO("Detected a total of {} AABB collisions in trigger", manifoldArray.size(), static_cast<ENTT_ID_TYPE>(e));

            for(int j {}; j < manifoldArray.size(); ++j) {
                btPersistentManifold *manifold = manifoldArray[j];

                for(int p {}; p < manifold->getNumContacts(); ++p) {
                    // return true;
                    //  TODO: here collision response
                }
            }

            // return false;
        }
    };

    constexpr float kFixedTimestep = 1.0f / 60.0f;

    commandProcessor.update(EntMan);
    dynamicsWorld->stepSimulation(deltatime, 5, kFixedTimestep);
    EntMan.forAllMatching<CTriggerVolume_t>(checkTriggerCollisions);
    EntMan.forAllMatching<CTransform_t, CRigidbody_t>(updateEntitiesTransform);
    dynamicsWorld->debugDrawWorld();
    uploadDebugDrawContext(EntMan);
}

void SPhysics_t::registerAddColliderToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    commandProcessor.add<AddColliderToWorldCommand_t>(e);
}

void SPhysics_t::registerAddRigidbodyToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    commandProcessor.add<AddRigidbodyToWorldCommand_t>(e);
}

void SPhysics_t::registerAddTriggerToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    commandProcessor.add<AddTriggerToWorldCommand_t>(e);
}

void SPhysics_t::registerAddCharacterToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    commandProcessor.add<AddCharacterToWorldCommand_t>(e);
}

void SPhysics_t::removeAndDeleteBodyFromWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    const ECS::EntityManager_t& EntMan = registry.ctx().at<ECS::EntityManager_t>();
    const CRigidbody_t& rigidbody = EntMan.getComponent<CRigidbody_t>(e);

    if(rigidbody.attachedCollider) {
        btCollisionShape* shape = static_cast<btCollisionShape*>(rigidbody.attachedCollider->runtimeCollisionShape);
        if(shape) delete shape;
    }
    
    btRigidBody* bullet3body = static_cast<btRigidBody*>(rigidbody.runtimeRigidbody);

    if(bullet3body) {
        if (bullet3body->getMotionState()) {
            delete bullet3body->getMotionState();
        }

        dynamicsWorld->removeRigidBody(bullet3body);

        RigidbodyUserPointer_t* userPointer = static_cast<RigidbodyUserPointer_t*>(bullet3body->getUserPointer());

        if(userPointer) {
            delete userPointer;
            LOG_CORE_INFO("User pointer freed from rigidbody");
        }

        delete bullet3body;
    }

    LOG_CORE_WARN("I got here, technically rigidbody runtime destruction is complete");
    // yeah it works well
}

// private functions

void SPhysics_t::initPhysicsContext() {
    m_PhysicsContext.dynamicsWorld = dynamicsWorld.get();
    m_PhysicsContext.triggers = &m_CachedTriggerObjects;
}

void SPhysics_t::uploadDebugDrawContext(ECS::EntityManager_t& EntMan) {
    // if(!debugDraw.get()) return;

    Bullet3PhysicsDrawer_t* bulletDrawer = (Bullet3PhysicsDrawer_t*)(debugDraw.get());
    SCPhysicsDrawingContext_t& context = EntMan.getSingletonComponent<SCPhysicsDrawingContext_t>();
    context.lines = &bulletDrawer->getLinesVector();
}

void SPhysics_t::bulletInternalTickCallback(btDynamicsWorld* world, float timeStep) {
    // TODO: setup bullet collision response, there are two ways (and I don't know which one's better)
    // 1st approach is to set an internal callback in the world that will check collisions manifolds
    // 2nd approach is to do the same manually after StepSimulation
    // I got a little doubt if the callback is called in the substepping process, if it does, 1st method might have too much calls
    // spoiler: it does get called, 1st method is the way to go

    SPhysics_t* Physics = static_cast<SPhysics_t*>(world->getWorldUserInfo());

    if(!Physics) return;
    // gContactAddedCallback
    // gContactEndedCallback
    // btBroadphaseProxy::SensorTrigger
    
    // LOG_CORE_CRITICAL("Internal Physics Callback Timestep: {}", timeStep);
    int numManifolds = world->getDispatcher()->getNumManifolds();
    // LOG_CORE_WARN("Manifold count: {}", numManifolds);

    // Check contacts between objects with contact response (rigidbodies, and maybe later generic collision objects)
    for (int i {}; i < numManifolds; ++i) {
        btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

        RigidbodyUserPointer_t* userPointerA = static_cast<RigidbodyUserPointer_t*>(obA->getUserPointer());
        RigidbodyUserPointer_t* userPointerB = static_cast<RigidbodyUserPointer_t*>(obB->getUserPointer());

        if(!userPointerA || !userPointerB) return;

        ECS::EntityManager_t& EntMan = *(userPointerA->entityManager);

        CRigidbody_t& rbA = *(userPointerA->rigidbodyComponent);
        CRigidbody_t& rbB = *(userPointerB->rigidbodyComponent);
        ECS::Entityid_t entityA = EntMan.getEntity(rbA);
        ECS::Entityid_t entityB = EntMan.getEntity(rbB);

        uint32_t idA { static_cast<ENTT_ID_TYPE>(entityA) }, idB { static_cast<ENTT_ID_TYPE>(entityB) };
        // LOG_CORE_INFO("Detected collision between entities {} and {}", idA, idB);

        // TODO: here I should save to a SPhysics_t member container a collection of all collisions 
        // (maybe a set to avoid duplicates, I dunno)
        // collisions being a struct holding the id's of entities collided and hitInfo data
        // like the contact points and their normals, etc

        // Up to this point is (at least) the broadphase acceptance of a collision, we don't know if it is a physical one
        int numContacts = contactManifold->getNumContacts();

        for (int j {}; j < numContacts; ++j) {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            
            // if any point exceeds the other's structure, there is a physical collision, not just AABB's
            if (pt.getDistance() < 0.0f) {
                // LOG_CORE_INFO("Detected Internal collision between entities {} and {}", idA, idB);
                break;
                
                // TODO: store contact information
                // const btVector3& ptA = pt.getPositionWorldOnA();
                // const btVector3& ptB = pt.getPositionWorldOnB();
                // const btVector3& normalOnB = pt.m_normalWorldOnB;
            }
        }
    }

    // Contact checks of ghost objects will happen during update
}

void SPhysics_t::bulletContactStartedCallback(btPersistentManifold* const& manifold) {
    const btCollisionObject* object0 = manifold->getBody0();
    const btCollisionObject* object1 = manifold->getBody1();

    if(!(object0 && object1)) return;

    RigidbodyUserPointer_t* userPointer0 = static_cast<RigidbodyUserPointer_t*>(object0->getUserPointer());
    RigidbodyUserPointer_t* userPointer1 = static_cast<RigidbodyUserPointer_t*>(object1->getUserPointer());

    if(!(userPointer0 && userPointer1)) return;

    ECS::EntityManager_t& EntMan = *(userPointer0->entityManager);
    ECS::Entityid_t entity0 = userPointer0->entity;
    ECS::Entityid_t entity1 = userPointer1->entity;

    // I don't need both entities to be collisionable for even a single one to call their callbacks
    // but test if both have at least one component to call their callbacks
    if(!(EntMan.hasComponent<CCollisionable_t>(entity0) || EntMan.hasComponent<CCollisionable_t>(entity1))) return;

    // Determine the type of collision, assume that CRigidbody_t will contain a btRigidbody and trigger volume a ghost object
    CollisionEventType_t eventType { CollisionEventType_t::VOID_COLLISION };

    if(EntMan.hasComponent<CRigidbody_t>(entity0) && EntMan.hasComponent<CRigidbody_t>(entity1)) {
        eventType = CollisionEventType_t::BODY_WITH_BODY;
    } else if (EntMan.hasComponent<CTriggerVolume_t>(entity0) && EntMan.hasComponent<CTriggerVolume_t>(entity1)) {
        eventType = CollisionEventType_t::TRIGGER_WITH_TRIGGER;
    } else if (
        (EntMan.hasComponent<CRigidbody_t>(entity0) || EntMan.hasComponent<CRigidbody_t>(entity1))
        && (EntMan.hasComponent<CTriggerVolume_t>(entity0) || EntMan.hasComponent<CTriggerVolume_t>(entity1)) 
    ) {
        eventType = CollisionEventType_t::BODY_WITH_TRIGGER;
    }

    if(eventType == CollisionEventType_t::VOID_COLLISION) return;

    CCollisionable_t* collisionable0 = EntMan.tryGetComponent<CCollisionable_t>(entity0);
    CCollisionable_t* collisionable1 = EntMan.tryGetComponent<CCollisionable_t>(entity1);
    CollisionEvent_t collision { eventType, EntMan, entity0, entity1 };

    // Send collision event to first collisionable
    // Check if callback is found in the map, else it could break the game

    if(collisionable0) { // Trigger callback if exists for that collision interaction
        auto& callbacks = collisionable0->callbacks;

        if((callbacks.find(collision.type) != callbacks.end())) {
            CCollisionable_t::CallbackCollection_t& events = callbacks.at(collision.type);

            if(events.onEventEnter) {
                events.onEventEnter(collision);
            }
        } 
    }

    // Swap pointers so collisionableA refers to self and collisionableB refers to the other
    std::swap(collision.selfEntity, collision.otherEntity);

    // Now sent it to the other

    if(collisionable1) { // Trigger callback if exists for that collision interaction
        auto& callbacks = collisionable1->callbacks;

        if((callbacks.find(collision.type) != callbacks.end())) {
            CCollisionable_t::CallbackCollection_t& events = callbacks.at(collision.type);

            if(events.onEventEnter) {
                events.onEventEnter(collision);
            }
        } 
    }
}

void SPhysics_t::bulletContactEndedCallback(btPersistentManifold* const& manifold) {
    // FIXME: it's the same as the contact start but calling other function callback inside the component
    // TODO: abstract the bulk of the code
    const btCollisionObject* object0 = manifold->getBody0();
    const btCollisionObject* object1 = manifold->getBody1();

    if(!(object0 && object1)) return;

    RigidbodyUserPointer_t* userPointer0 = static_cast<RigidbodyUserPointer_t*>(object0->getUserPointer());
    RigidbodyUserPointer_t* userPointer1 = static_cast<RigidbodyUserPointer_t*>(object1->getUserPointer());

    if(!(userPointer0 && userPointer1)) return;

    ECS::EntityManager_t& EntMan = *(userPointer0->entityManager);
    ECS::Entityid_t entity0 = userPointer0->entity;
    ECS::Entityid_t entity1 = userPointer1->entity;

    // I don't need both entities to be collisionable for even a single one to call their callbacks
    // but test if both have at least one component to call their callbacks
    if(!(EntMan.hasComponent<CCollisionable_t>(entity0) || EntMan.hasComponent<CCollisionable_t>(entity1))) return;

    // Determine the type of collision, assume that CRigidbody_t will contain a btRigidbody and trigger volume a ghost object
    CollisionEventType_t eventType { CollisionEventType_t::VOID_COLLISION };

    if(EntMan.hasComponent<CRigidbody_t>(entity0) && EntMan.hasComponent<CRigidbody_t>(entity1)) {
        eventType = CollisionEventType_t::BODY_WITH_BODY;
    } else if (EntMan.hasComponent<CTriggerVolume_t>(entity0) && EntMan.hasComponent<CTriggerVolume_t>(entity1)) {
        eventType = CollisionEventType_t::TRIGGER_WITH_TRIGGER;
    } else if (
        (EntMan.hasComponent<CRigidbody_t>(entity0) || EntMan.hasComponent<CRigidbody_t>(entity1))
        && (EntMan.hasComponent<CTriggerVolume_t>(entity0) || EntMan.hasComponent<CTriggerVolume_t>(entity1)) 
    ) {
        eventType = CollisionEventType_t::BODY_WITH_TRIGGER;
    }

    if(eventType == CollisionEventType_t::VOID_COLLISION) return;

    CCollisionable_t* collisionable0 = EntMan.tryGetComponent<CCollisionable_t>(entity0);
    CCollisionable_t* collisionable1 = EntMan.tryGetComponent<CCollisionable_t>(entity1);
    CollisionEvent_t collision { eventType, EntMan, entity0, entity1 };

    // Send collision event to first collisionable
    // Check if callback is found in the map, else it could break the game

    if(collisionable0) { // Trigger callback if exists for that collision interaction
        auto& callbacks = collisionable0->callbacks;

        if((callbacks.find(collision.type) != callbacks.end())) {
            CCollisionable_t::CallbackCollection_t& events = callbacks.at(collision.type);

            if(events.onEventExit) {
                events.onEventExit(collision);
            }
        } 
    }

    // Swap pointers so collisionableA refers to self and collisionableB refers to the other
    std::swap(collision.selfEntity, collision.otherEntity);

    // Now sent it to the other

    if(collisionable1) { // Trigger callback if exists for that collision interaction
        auto& callbacks = collisionable1->callbacks;

        if((callbacks.find(collision.type) != callbacks.end())) {
            CCollisionable_t::CallbackCollection_t& events = callbacks.at(collision.type);

            if(events.onEventExit) {
                events.onEventExit(collision);
            }
        } 
    }
}

// Private Anonymous Functions

btRigidBody& GetBullet3Rigidbody(const CRigidbody_t& rigidbody) {
    btRigidBody& bullet3body = *(static_cast<btRigidBody*>(rigidbody.runtimeRigidbody));
    return bullet3body;
}

// Public Anonymous Functions TODO: there are more, add all of them

void ApplyForce(CRigidbody_t& rigidbody, const Vector3f_t& force, const Vector3f_t& relativePosition) {
    btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);
    bullet3body.applyForce(force, relativePosition);
}

void ApplyCentralForce(CRigidbody_t& rigidbody, const Vector3f_t& force) {
    btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);
    bullet3body.applyCentralForce(force);
}

void ApplyTorque(CRigidbody_t& rigidbody, const Vector3f_t& torque) {
    btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);
    bullet3body.applyTorque(torque);
}

void ApplyImpulse(CRigidbody_t& rigidbody, const Vector3f_t& impulse, const Vector3f_t& relativePosition) {
    btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);
    bullet3body.applyImpulse(impulse, relativePosition);
}

void ApplyCentralImpulse(CRigidbody_t& rigidbody, const Vector3f_t& force) {
    btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);
    bullet3body.applyCentralImpulse(force);
}

void SetLinearVelocity(CRigidbody_t& rigidbody, const Vector3f_t& velocity) {
    btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);
    bullet3body.setLinearVelocity(velocity);
}

void SetAngularVelocity(CRigidbody_t& rigidbody, const Vector3f_t& velocity) {
    btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);
    bullet3body.setAngularVelocity(velocity);
}

void Translate(CRigidbody_t& rigidbody, const Vector3f_t& movement) {
    btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);
    bullet3body.translate(movement);
}

void ClearForces(CRigidbody_t& rigidbody) {
    btRigidBody& bullet3body = GetBullet3Rigidbody(rigidbody);
    bullet3body.clearForces();
}
