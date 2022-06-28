#include "factory.hpp"
#include <iostream>

EntityFactory_t::EntityFactory_t(ECS::EntityManager_t& pEntMan) : EntMan(pEntMan) {}

// ECS::Entityid_t EntityFactory_t::createPlayer(const Vector3f& position) {
//     ECS::Entityid_t e = EntMan.createEntity();

//     {
//         CTransform_t& transform = EntMan.addComponent<CTransform_t>(e);
//         transform.position = position;
//     }
//     {
//         CModelRenderer_t& model = EntMan.addComponent<CModelRenderer_t>(e);
//         model.model = RL::LoadModel("assets/untitled.obj");
//     }

//     return e;
// }

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


// ECS::Entityid_t EntityFactory_t::createCamera(const Vector3f& position) {
//     ECS::Entityid_t e = EntMan.createEntity();

//     {
//         CTransform_t& transform = EntMan.addComponent<CTransform_t>(e);
//         transform.position = position;
//     }
//     {
//         CCamera_t& camera = EntMan.addComponent<CCamera_t>(e);
//         camera.fovy = 90.0f;
//     }

//     return e;
// }

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
        camera.fovy = 60.0f;
    }
    {
        CInput_t& input  = EntMan.addComponent<CInput_t>(e);
        {
            MouseDeltaAction_t action {
                [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e, const Vector2f_t vector) {
                    const float sensibility = 0.1f;
                    CTransform_t& transform = EntMan.getComponent<CTransform_t>(e);
                    transform.rotation.add_x(vector.get_y() * sensibility * -1.0f);
                    transform.rotation.add_y(vector.get_x() * sensibility * -1.0f);
                }
            };

            input.mouseDeltaActions.emplace_back(action);
        }
        {
            InputAction_t action { 
                Key_t::W
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                    auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                    transform.position += transform.rotation.forward();
                } 
            };

            input.actions.emplace_back(action);
        }
        {
            InputAction_t action { 
                Key_t::A
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                    auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                    transform.position -= transform.rotation.right();
                } 
            };

            input.actions.emplace_back(action);
        }
        {
            InputAction_t action { 
                Key_t::S
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                    auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                    transform.position -= transform.rotation.forward();
                } 
            };

            input.actions.emplace_back(action);
        }
        {
            InputAction_t action { 
                Key_t::D
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                    auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                    transform.position += transform.rotation.right();
                } 
            };

            input.actions.emplace_back(action);
        }
        {
            InputAction_t action { 
                Key_t::SPACE
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                    auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                    transform.position += Vector3f_t{0,1,0};
                } 
            };

            input.actions.emplace_back(action);
        }
        {
            InputAction_t action { 
                Key_t::LEFT_CONTROL
            ,   KeyState_t::DOWN
            ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                    auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                    transform.position -= Vector3f_t{0,1,0};
                } 
            };

            input.actions.emplace_back(action);
        }

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

