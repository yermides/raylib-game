#include "factory.hpp"
#include "game/sys/helpers/all.hpp"
#include <iostream>

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
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
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
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
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
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
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
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                    CTransform_t& transform = EntMan.getComponent<CTransform_t>(e);
                    float xposition = transform.position.get_x() - .3f;
                    transform.position.set_x(xposition);
                } 
            };

            input.actions.emplace_back(action);
        }
    }

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

    return e;
}

ECS::Entityid_t EntityFactory_t::createStaticMesh(std::string_view filepath, const CTransform_t& ptransform) {
    ECS::Entityid_t e = EntMan.createEntity();

    {
        CTransform_t& transform = EntMan.addComponent<CTransform_t>(e);
        transform = ptransform;
    }
    {
        CModelRenderer_t& model = EntMan.addComponent<CModelRenderer_t>(e);
        model.model = RL::LoadModel(filepath.data());
    }

    return e;
}


ECS::Entityid_t EntityFactory_t::createPhysicsPlane(const CTransform_t& ptransform) { 
    ECS::Entityid_t e = EntMan.createEntity();

    EntMan.addComponent<CTransform_t>(e, ptransform);

    {
        CBoxCollider_t& collider = EntMan.addComponent<CBoxCollider_t>(e);
        collider.boxHalfExtents = Vector3f_t{ 20.0f, 2.0f, 20.0f };
    }
    {
        CRigidbody_t& body = EntMan.addComponent<CRigidbody_t>(e);
        body.type = BodyType_t::STATIC;
    }

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

    return e;
}

ECS::Entityid_t EntityFactory_t::createCharacter(const CTransform_t& ptransform) {
    ECS::Entityid_t e = EntMan.createEntity();

    EntMan.addComponent<CTransform_t>(e, ptransform);

    {
        CCapsuleCollider_t& collider = EntMan.addComponent<CCapsuleCollider_t>(e);
        collider.radius = 2.0f;
        collider.height = 4.0f;
    }
    // {
    //     CModelRenderer_t& model = EntMan.addComponent<CModelRenderer_t>(e);
    //     model.model = RL::LoadModel("assets/untitled.obj");
    // }
    {
        CRigidbody_t& body = EntMan.addComponent<CRigidbody_t>(e);
        body.type = BodyType_t::KINEMATIC;
        body.angularFactor = {0,0,0};
        // body.type = BodyType_t::DYNAMIC;
        // body.mass = 0.1f;
        // body.angularFactor = {1,0,0};
    }

    return e;
}

        // CCharacterController_t& controller = EntMan.addComponent<CCharacterController_t>(e);