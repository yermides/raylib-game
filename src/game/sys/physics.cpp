#include "physics.hpp"
#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet3/btBulletDynamicsCommon.h>
#include "helpers/vector3.hpp"
#include "game/cmp/helpers/all.hpp"
#include "helpers/physics_drawer.hpp"
#include "helpers/logger.hpp"

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

    dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    dynamicsWorld->setGravity(gravity);

    debugDraw = std::make_unique<Bullet3PhysicsDrawer_t>();
    dynamicsWorld->setDebugDrawer(debugDraw.get());

    // TODO: setup bullet collision response, there are two ways (and I don't know which one's better)
    // 1st approach is to set an internal callback in the world that will check collisions manifolds
    // 2nd approach is to do the same manually after StepSimulation
    // I got a little doubt if the callback is called in the substepping process, if it does, 1st method might have too much calls
    // spoiler: it does get called, 1st method is the way to go

    dynamicsWorld->setInternalTickCallback(bulletInternalTickCallback, this, true);

    m_PhysicsContext.dynamicsWorld = dynamicsWorld.get();
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

void SPhysics_t::update(ECS::EntityManager_t& EntMan, const float deltatime) {
    auto lambda = [](auto e, CTransform_t& transform, CRigidbody_t& rigidbody) {
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

    constexpr float kFixedTimestep = 1.0f / 60.0f;

    commandProcessor.update(EntMan);

    // addCollidersToWorld(EntMan);
    // addRigidbodiesToWorld(EntMan); // maybe rename to addRigidbodiesToWorld
    // addCharactersToWorld(EntMan);

    dynamicsWorld->stepSimulation(deltatime, 5, kFixedTimestep);
    EntMan.forAllMatching<CTransform_t, CRigidbody_t>(lambda);
    dynamicsWorld->debugDrawWorld();
    uploadDebugDrawContext(EntMan);
}

void SPhysics_t::ping() {
    LOG_INFO("Yes, I am alive {}", (void*)this);
}

void SPhysics_t::registerAddColliderToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    commandProcessor.add<AddColliderToWorldCommand_t>(e);
    return;
    collidersToAddWorld.emplace_back(e);
}

void SPhysics_t::registerAddRigidbodyToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    commandProcessor.add<AddRigidbodyToWorldCommand_t>(e);
    return;
    rigidbodiesToAddWorld.emplace_back(e);
}

void SPhysics_t::registerAddTriggerToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    triggersToAddWorld.emplace_back(e);
}

void SPhysics_t::registerAddCharacterToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    charactersToAddWorld.emplace_back(e);
}

void SPhysics_t::removeAndDeleteBodyFromWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    const ECS::EntityManager_t& EntMan = registry.ctx().at<ECS::EntityManager_t>();
    const CRigidbody_t& rigidbody = EntMan.getComponent<CRigidbody_t>(e);

    if(rigidbody.attachedCollider) {
        btCollisionShape* shape = static_cast<btCollisionShape*>(rigidbody.attachedCollider->runtimeBullet3CollisionShape);
        if(shape) delete shape;
    }
    
    btRigidBody* bullet3body = static_cast<btRigidBody*>(rigidbody.runtimeBullet3Body);

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

template<typename ColliderType>
void* GetColliderInternalData(ColliderType& collider) {}

template<>
void* GetColliderInternalData(CBoxCollider_t& collider) {
    return collider.runtimeBullet3CollisionShape;
}

template<>
void* GetColliderInternalData(CSphereCollider_t& collider) {
    return collider.runtimeBullet3CollisionShape;
}

template<>
void* GetColliderInternalData(CCapsuleCollider_t& collider) {
    return collider.runtimeBullet3CollisionShape;
}

// private functions
void SPhysics_t::addCollidersToWorld(ECS::EntityManager_t& EntMan) {

    for(const ECS::Entityid_t& e : collidersToAddWorld) {
        if(!EntMan.hasAnyComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(e)) continue;

        CCollider_t*      colliderComponent {};
        btCollisionShape* collisionShape    {};

        if(EntMan.hasComponent<CBoxCollider_t>(e)) {
            CBoxCollider_t& collider = EntMan.getComponent<CBoxCollider_t>(e);
            collisionShape = new btBoxShape(collider.boxHalfExtents);
            colliderComponent = &collider;
        } else if (EntMan.hasComponent<CSphereCollider_t>(e)) {
            CSphereCollider_t& collider = EntMan.getComponent<CSphereCollider_t>(e);
            collisionShape = new btSphereShape(collider.radius);
            colliderComponent = &collider;
        } else if (EntMan.hasComponent<CCapsuleCollider_t>(e)) {
            CCapsuleCollider_t& collider = EntMan.getComponent<CCapsuleCollider_t>(e);
            collisionShape = new btCapsuleShape(collider.radius, collider.height);
            colliderComponent = &collider;
        }

        colliderComponent->runtimeBullet3CollisionShape = collisionShape;

        // I'm still doubting these last lines, rigidbody may check ifthere's a collider, but anyway
        // if(EntMan.hasComponent<CRigidbody_t>(e)) {
        //     CRigidbody_t& rigidbody = EntMan.getComponent<CRigidbody_t>(e);
        //     rigidbody.attachedCollider = colliderComponent;
        // }
    }

    collidersToAddWorld.clear();
}

void SPhysics_t::addRigidbodiesToWorld(ECS::EntityManager_t& EntMan) {


    for(const ECS::Entityid_t& e : rigidbodiesToAddWorld) {
        if(!EntMan.hasComponent<CRigidbody_t>(e) 
        || !EntMan.hasAnyComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(e)) {
            continue; // TODO: improve if so it doesn't completely ignore if it doesn't have shape
        }

        // generate shape & body, and add body to world

        CCollider_t* col {}; // morph depending on the type

        auto [transform, rigidbody] = EntMan.getComponents<CTransform_t, CRigidbody_t>(e);
        btCollisionShape* collisionShape {};

        // TODO: could be improved with template partial specialization or another trigger (in process)
        if(EntMan.hasComponent<CBoxCollider_t>(e)) {
            CBoxCollider_t& collider = EntMan.getComponent<CBoxCollider_t>(e);
            col = &collider;
            collisionShape = new btBoxShape(collider.boxHalfExtents);
            LOG_CORE_INFO("CollisionShape created as CBoxCollider_t");
        } else if (EntMan.hasComponent<CSphereCollider_t>(e)) {
            CSphereCollider_t& collider = EntMan.getComponent<CSphereCollider_t>(e);
            col = &collider;
            collisionShape = new btSphereShape(collider.radius);
            LOG_CORE_INFO("CollisionShape created as CSphereCollider_t");
        } else if (EntMan.hasComponent<CCapsuleCollider_t>(e)) {
            CCapsuleCollider_t& collider = EntMan.getComponent<CCapsuleCollider_t>(e);
            col = &collider;
            collisionShape = new btCapsuleShape(collider.radius, collider.height);
            LOG_CORE_INFO("CollisionShape created as CCapsuleCollider_t");
        }

        // Set rigidbody's translation and rotation upon construction in the same place as the entity
        //  TODO: maybe use offset
        
        btTransform bulletTransform;
        {   // DONE: mimic rotation of transform.rotation
            bulletTransform.setIdentity(); 
            bulletTransform.setOrigin(transform.position);

            btQuaternion quat;
            const Vector3f_t& rot = transform.rotation;
            quat.setEuler(rot.get_y(), rot.get_x(), rot.get_z());
            bulletTransform.setRotation(quat);
        }

        // for bullet, bodies are automatically dynamic if they have mass, so enforce mass if type dynamic
        if(rigidbody.type == BodyType_t::DYNAMIC) {
            if(rigidbody.mass == 0.0f) {
                rigidbody.mass = 1.0f;
            }
        } else { // also do the inverse if not dynamic
            if(rigidbody.mass > 0.0f) {
                rigidbody.mass = 0.0f;
            }
        }

        btScalar mass(rigidbody.mass);
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);

        if (isDynamic) {
            collisionShape->calculateLocalInertia(mass, localInertia);
        }


        // Using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(bulletTransform);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);

        btRigidBody* body = new btRigidBody(rbInfo);
        body->setAngularFactor(rigidbody.angularFactor);

        if(rigidbody.type == BodyType_t::STATIC) {
	        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT); 
        } else if(rigidbody.type == BodyType_t::KINEMATIC) {
	        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); 
            
            // in reality needed but if done, I'd need to override btDefaultMotionState with my own, see bullet docs p21 & p22
	        // body->setActivationState(DISABLE_DEACTIVATION); 
        } else {
	        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DYNAMIC_OBJECT); 
        }

        // TODO: collision flags and group

        col->runtimeBullet3CollisionShape = collisionShape;
        rigidbody.runtimeBullet3Body = body;
        rigidbody.attachedCollider = col;

        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body ); //, mask, group

        RigidbodyUserPointer_t* userPointer = new RigidbodyUserPointer_t {
            &EntMan, &rigidbody
        };

        body->setUserPointer(userPointer);

        LOG_CORE_INFO("Rigidbody successfully created and added to world");
    }

    rigidbodiesToAddWorld.clear();

    
}

void SPhysics_t::addTriggersToWorld(ECS::EntityManager_t& EntMan) {
    // FIXME: not use until it's functional, there's a rework of collision shapes addition in progress 
    for(const ECS::Entityid_t& e : triggersToAddWorld) {
        if(!EntMan.hasComponent<CTriggerVolume_t>(e) 
        || !EntMan.hasAnyComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(e)) {
            continue;
        }


    }

    triggersToAddWorld.clear();

    // just keeping what should be inside the loop in here
    // FIXME: erase this once I finish testing

    btBoxShape* boxShape = new btBoxShape(btVector3(5,5,5));
    btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();

    // TODO: learn the difference between regular and pair caching ghost object
    // btGhostObject* ghostObject = new btGhostObject();

    btTransform planeTransform;
    planeTransform.setIdentity();
    planeTransform.setOrigin(btVector3(0, 10, 0));

    ghostObject->setCollisionShape(boxShape);
    ghostObject->setWorldTransform(planeTransform);
    dynamicsWorld->addCollisionObject(ghostObject);

    // makes it so it's not solid but receives collision events (just like a sensor/trigger yay!)
    ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    // TODO: also, collisions have to be tested each frame, so I should cache all triggers and check for all of them
    // their collisions inside the InternalTickCallback
}

void SPhysics_t::addCharactersToWorld(ECS::EntityManager_t& EntMan) {
    for(const ECS::Entityid_t& e : charactersToAddWorld) {
        if(!EntMan.hasAllComponents<CCapsuleCollider_t, CCharacterController_t>(e)) { continue; }

        // auto [collider, controller] = EntMan.getComponents<CCapsuleCollider_t, CCharacterController_t>(e);
        // TODO: continue
    }

    charactersToAddWorld.clear();
}

void SPhysics_t::uploadDebugDrawContext(ECS::EntityManager_t& EntMan) {
    // if(!debugDraw.get()) return;

    Bullet3PhysicsDrawer_t* bulletDrawer = (Bullet3PhysicsDrawer_t*)(debugDraw.get());
    SCPhysicsDrawingContext_t& context = EntMan.getSingletonComponent<SCPhysicsDrawingContext_t>();
    context.lines = &bulletDrawer->getLinesVector();
}

void SPhysics_t::bulletInternalTickCallback(btDynamicsWorld* world, float timeStep) {
    SPhysics_t* Physics = static_cast<SPhysics_t*>(world->getWorldUserInfo());

    if(!Physics) return;
    // gContactAddedCallback
    // gContactEndedCallback

    // btBroadphaseProxy::SensorTrigger
    
    // static int count = 0;
    // LOG_CORE_CRITICAL("Internal Physics Callback Timestep: {}", timeStep);
    int numManifolds = world->getDispatcher()->getNumManifolds();
    // LOG_CORE_WARN("Manifold count: {}", numManifolds);


    // LOG_CORE_WARN("=============================================");
    for (int i {}; i < numManifolds; ++i) {
        btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
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
        LOG_CORE_INFO("Detected collision between entities {} and {}", idA, idB);

        // LOG_CORE_INFO("Detected collision between entities {} and {}", static_cast<ENTT_ID_TYPE>(entityA), static_cast<ENTT_ID_TYPE>(entityB));

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
                LOG_CORE_INFO("Detected Internal collision between entities {} and {}", idA, idB);
                break;

                
                // TODO: store contact information
                // const btVector3& ptA = pt.getPositionWorldOnA();
                // const btVector3& ptB = pt.getPositionWorldOnB();
                // const btVector3& normalOnB = pt.m_normalWorldOnB;
            }
        }
    }

    // LOG_CORE_WARN("=============================================");
}

// Private Anonymous Functions

btRigidBody& GetBullet3Rigidbody(const CRigidbody_t& rigidbody) {
    btRigidBody& bullet3body = *(static_cast<btRigidBody*>(rigidbody.runtimeBullet3Body));
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
