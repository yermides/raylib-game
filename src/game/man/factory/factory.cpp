#include "factory.hpp"
#include "game/cmp/helpers/all.hpp"
#include "game/sys/helpers/all.hpp"
#include "game/helpers/logger.hpp"

EntityFactory_t::EntityFactory_t(ECS::EntityManager_t& pEntMan) : EntMan(pEntMan) {}

ECS::Entityid_t EntityFactory_t::createPlayer(const CTransform_t& ptransform) {
    ECS::Entityid_t e = EntMan.createEntity();

    {
        EntMan.addComponent<CTransform_t>(e, ptransform); // using copy constructor implicitely
    }
    {
        CModelRenderer_t& model = EntMan.addComponent<CModelRenderer_t>(e);
        model.model = RL::LoadModel("assets/untitled.obj");
    }
    {
        CInput_t& input  = EntMan.addComponent<CInput_t>(e);

        {
            InputAction_t action { 
                Key_t::W
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e, const float deltatime) {
                    CTransform_t& transform = EntMan.getComponent<CTransform_t>(e);
                    float zposition = transform.position.get_z() + .3f;
                    transform.position.set_z(zposition);
                } 
            };

            input.actions.emplace_back(action);
        }

        {
            InputAction_t action { 
                Key_t::S
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e, const float deltatime) {
                    CTransform_t& transform = EntMan.getComponent<CTransform_t>(e);
                    float zposition = transform.position.get_z() - .3f;
                    transform.position.set_z(zposition);
                } 
            };

            input.actions.emplace_back(action);
        }

        {
            InputAction_t action { 
                Key_t::A
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e, const float deltatime) {
                    CTransform_t& transform = EntMan.getComponent<CTransform_t>(e);
                    float xposition = transform.position.get_x() + .3f;
                    transform.position.set_x(xposition);
                } 
            };

            input.actions.emplace_back(action);
        }

        {
            InputAction_t action { 
                Key_t::D
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e, const float deltatime) {
                    CTransform_t& transform = EntMan.getComponent<CTransform_t>(e);
                    float xposition = transform.position.get_x() - .3f;
                    transform.position.set_x(xposition);
                } 
            };

            input.actions.emplace_back(action);
        }
    }

    LOG_INFO(std::string(__PRETTY_FUNCTION__) + " = {};", static_cast<ENTT_ID_TYPE>(e));
    return e;
}

ECS::Entityid_t EntityFactory_t::createCamera(const CTransform_t& ptransform) {
    ECS::Entityid_t e = EntMan.createEntity();

    {
        EntMan.addComponent<CTransform_t>(e, ptransform);
    }
    {
        CCamera_t& camera = EntMan.addComponent<CCamera_t>(e);
        camera.fovy = 90.0f;
    }

    LOG_INFO(std::string(__PRETTY_FUNCTION__) + " = {};", static_cast<ENTT_ID_TYPE>(e));
    return e;
}

ECS::Entityid_t EntityFactory_t::createFlyingCamera(const CTransform_t& ptransform) {
    ECS::Entityid_t e = EntMan.createEntity();

    {
        EntMan.addComponent<CTransform_t>(e, ptransform); // using copy constructor implicitely
    }
    {
        CCamera_t& camera = EntMan.addComponent<CCamera_t>(e);
        camera.fovy = 90.0f;
    }
    {
        EntMan.addComponent<CInput_t>(e, CreateFlyingCameraControls());
    }

    LOG_INFO(std::string(__PRETTY_FUNCTION__) + " = {};", static_cast<ENTT_ID_TYPE>(e));
    return e;
}

ECS::Entityid_t EntityFactory_t::createStaticMesh(const CTransform_t& ptransform, std::string_view filepath) {
    ECS::Entityid_t e = EntMan.createEntity();

    {
        CTransform_t& transform = EntMan.addComponent<CTransform_t>(e);
        transform = ptransform;
    }
    {
        CModelRenderer_t& model = EntMan.addComponent<CModelRenderer_t>(e);
        model.model = RL::LoadModel(filepath.data());
    }

    LOG_INFO(std::string(__PRETTY_FUNCTION__) + " = {};", static_cast<ENTT_ID_TYPE>(e));
    return e;
}


ECS::Entityid_t EntityFactory_t::createPhysicsPlane(const CTransform_t& ptransform) { 
    ECS::Entityid_t e = EntMan.createEntity();

    EntMan.addComponent<CTransform_t>(e, ptransform);

    {
        CBoxCollider_t& collider = EntMan.addComponent<CBoxCollider_t>(e);
        collider.boxHalfExtents = Vector3f_t{ 50.0f, 2.0f, 50.0f };
    }
    {
        CRigidbody_t& body = EntMan.addComponent<CRigidbody_t>(e);
        body.type = BodyType_t::STATIC;
    }
    {
        CCollisionable_t& collisionable = EntMan.addComponent<CCollisionable_t>(e);
        collisionable.type = CollisionableType_t::Platform;

        CCollisionable_t::CallbackCollection_t bodyWithBodyCollection;
        bodyWithBodyCollection.onEventEnter = [](const CollisionEvent_t& collision) {
            LOG_CORE_WARN("Plane with id {} has been in contact with {}", (uint32_t)collision.selfEntity, (uint32_t)collision.otherEntity);
        };

        collisionable.callbacks[CollisionEventType_t::BODY_WITH_BODY] = bodyWithBodyCollection;
    }

    LOG_INFO(std::string(__PRETTY_FUNCTION__) + " = {};", static_cast<ENTT_ID_TYPE>(e));
    return e;
}

ECS::Entityid_t EntityFactory_t::createPhysicsBall(const CTransform_t& ptransform) { 
    ECS::Entityid_t e = EntMan.createEntity();

    EntMan.addComponent<CTransform_t>(e, ptransform);

    {
        CSphereCollider_t& collider = EntMan.addComponent<CSphereCollider_t>(e);
        collider.radius = 2.0f;
    }
    // {
    //     CModelRenderer_t& model = EntMan.addComponent<CModelRenderer_t>(e);
    //     model.model = RL::LoadModel("assets/untitled.obj");
    // }
    {   
        CRigidbody_t& body = EntMan.addComponent<CRigidbody_t>(e);
        body.type = BodyType_t::DYNAMIC;
    }

    LOG_INFO(std::string(__PRETTY_FUNCTION__) + " = {};", static_cast<ENTT_ID_TYPE>(e));
    return e;
}

ECS::Entityid_t EntityFactory_t::createCharacter(const CTransform_t& ptransform) {
    ECS::Entityid_t e = EntMan.createEntity();

    EntMan.addComponent<CTransform_t>(e, ptransform);

    // {
    //     CModelRenderer_t& model = EntMan.addComponent<CModelRenderer_t>(e);
    //     model.model = RL::LoadModel("assets/untitled.obj");
    // }
    {
        CCapsuleCollider_t& collider = EntMan.addComponent<CCapsuleCollider_t>(e);
        collider.radius = 2.0f;
        collider.height = 6.0f;
    }
    {
        CRigidbody_t& body = EntMan.addComponent<CRigidbody_t>(e);
        {
            // body.angularFactor = {0,0,0};
            // body.type = BodyType_t::KINEMATIC;
        }
        {
            body.type = BodyType_t::DYNAMIC;
            body.mass = 10.0f;
            body.angularFactor = {0,0,0};
        }
    }
    {
        CCollisionable_t& collisionable = EntMan.addComponent<CCollisionable_t>(e);
        collisionable.type = CollisionableType_t::Player;

        CCollisionable_t::CallbackCollection_t bodyWithBodyCollection;
        bodyWithBodyCollection.onEventEnter = [](const CollisionEvent_t& collision) {
            ECS::EntityManager_t& EntMan = collision.EntMan;

            if(EntMan.hasComponent<CCollisionable_t>(collision.otherEntity)) {
                CCollisionable_t& otherCollisionable = EntMan.getComponent<CCollisionable_t>(collision.otherEntity);

                if(otherCollisionable.type == CollisionableType_t::Platform) {
                    CCharacterMovement_t& movement = EntMan.addComponent<CCharacterMovement_t>(collision.selfEntity);
                    movement.bCanJump = true;

                    LOG_CORE_WARN("Character with id {} should be able to jump", (uint32_t)collision.selfEntity);
                }
            }

            LOG_CORE_WARN("Character with id {} has been in contact with {}", (uint32_t)collision.selfEntity, (uint32_t)collision.otherEntity);
        };

        collisionable.callbacks[CollisionEventType_t::BODY_WITH_BODY] = bodyWithBodyCollection;
    }
    {
        CCharacterMovement_t& movement = EntMan.addComponent<CCharacterMovement_t>(e);
        movement.bCanJump = false;
        movement.fJumpForce = 300.0f;
    }
    {
        CInput_t input = EntMan.addComponent<CInput_t>(e, CreateThirdPersonCharacterControls());
    }

    LOG_INFO(std::string(__PRETTY_FUNCTION__) + " = {};", static_cast<ENTT_ID_TYPE>(e));
    return e;
}

    // CCharacterController_t& controller = EntMan.addComponent<CCharacterController_t>(e);

ECS::Entityid_t EntityFactory_t::createTrigger(const CTransform_t& ptransform) {
    ECS::Entityid_t e = EntMan.createEntity();

    EntMan.addComponent<CTransform_t>(e, ptransform);

    CSphereCollider_t& collider = EntMan.addComponent<CSphereCollider_t>(e);
    collider.radius = 5.0f;

    EntMan.addComponent<CTriggerVolume_t>(e);

    return e;
}
