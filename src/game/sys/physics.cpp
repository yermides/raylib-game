#include "physics.hpp"
#include "game/cmp/helpers/all.hpp"
#include "helpers/physics_drawer.hpp"

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

    dynamicsWorld->setGravity({ gravity.get_x(), gravity.get_y(), gravity.get_z() });

    debugDraw = std::make_unique<Bullet3PhysicsDrawer_t>();
    dynamicsWorld->setDebugDrawer(debugDraw.get());
}

SPhysics_t::~SPhysics_t() {
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
    auto lambda = [](auto e, CTransform_t& transform, const CRigidbody_t& rigidbody) {};

    addEntitiesToWorld(EntMan); // maybe rename to addRigidbodiesToWorld
    addCharactersToWorld(EntMan);
    dynamicsWorld->stepSimulation(deltatime, 10);

    EntMan.forAllMatching<CTransform_t, const CRigidbody_t>(lambda);

    dynamicsWorld->debugDrawWorld();
    uploadDebugDrawContext(EntMan);
}

void SPhysics_t::registerAddToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    entitiesToAddWorld.emplace_back(e);
}

void SPhysics_t::registerAddCharacterToWorld(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    charactersToAddWorld.emplace_back(e);
}

// private functions

void SPhysics_t::addEntitiesToWorld(ECS::EntityManager_t& EntMan) {
    for(const ECS::Entityid_t& e : entitiesToAddWorld) {
        if(!EntMan.hasComponent<CRigidbody_t>(e) 
        || !EntMan.hasAnyComponents<CBoxCollider_t, CSphereCollider_t, CCapsuleCollider_t>(e)) {
            continue; // TODO: improve if so it doesn't completely ignore if it doesn't have shape
        }

        // generate shape & body, and add body to world

        CCollider_t* col {}; // morph depending on the type

        auto [transform, rigidbody] = EntMan.getComponents<CTransform_t, CRigidbody_t>(e);
        btCollisionShape* collisionShape {};

        // TODO: could be improved with template partial specialization
        if(EntMan.hasComponent<CBoxCollider_t>(e)) {
            CBoxCollider_t& collider = EntMan.getComponent<CBoxCollider_t>(e);
            col = &collider;
            collisionShape = new btBoxShape(collider.boxHalfExtents);
            std::cout << "CBoxCollider_t\n";
        } else if (EntMan.hasComponent<CSphereCollider_t>(e)) {
            CSphereCollider_t& collider = EntMan.getComponent<CSphereCollider_t>(e);
            col = &collider;
            collisionShape = new btSphereShape(collider.radius);
            std::cout << "CSphereCollider_t\n";
        } else if (EntMan.hasComponent<CCapsuleCollider_t>(e)) {
            CCapsuleCollider_t& collider = EntMan.getComponent<CCapsuleCollider_t>(e);
            col = &collider;
            collisionShape = new btCapsuleShape(collider.radius, collider.height);
            std::cout << "CCapsuleCollider_t\n";
        }
        
        btTransform bulletTransform;
        bulletTransform.setIdentity(); // TODO: mimic rotation of transform.rotation
        bulletTransform.setOrigin(transform.position);

        // for bullet, bodies are automatically dynamic if they have mass, so enforce mass if type dynamic
        if(rigidbody.type == BodyType_t::DYNAMIC) {
            if(rigidbody.mass == 0.0f) {
                rigidbody.mass = 1.0f;
            }
        } else { // also do the inverse if not kinematic
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

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(bulletTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        body->setAngularFactor(rigidbody.angularFactor);

        col->runtimeBullet3CollisionShape = collisionShape;
        rigidbody.runtimeBullet3Body = body;

        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }

    entitiesToAddWorld.clear();
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
    SCPhysicsDrawingContext_t& context = EntMan.getSingletonComponent<SCPhysicsDrawingContext_t>();
    Bullet3PhysicsDrawer_t* bulletDrawer = (Bullet3PhysicsDrawer_t*)(debugDraw.get());
    context.lines = &bulletDrawer->getLinesVector();
}
