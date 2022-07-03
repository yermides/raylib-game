#include "game.hpp"
#include <iostream>
#include <imgui/imgui.h>
#include "game/cmp/helpers/all.hpp"
#include "helpers/includes/raylib.hpp"
#include "helpers/vector3.hpp"

Game_t::Game_t() {
    // Vector3f_t v1 {1,1,1};
    // Vector3f_t v2 {2,3,4};
    // std::cout << "Hello from Game!\n";
    // std::cout << ImGui::GetVersion() << "\n";
    // Vector3f_t::print(v1 = v2);

    EntMan.connectOnContruct<CCamera_t, &SRender_t::setMainCamera>(Render);
    EntMan.connectOnContruct<CRigidbody_t, &SPhysics_t::registerAddToWorld>(Physics);
    EntMan.connectOnContruct<CCharacterController_t, &SPhysics_t::registerAddCharacterToWorld>(Physics);
    EntMan.connectOnRemove<CModelRenderer_t, &SRender_t::unloadModel>(Render);

    // eCamera = Factory.createCamera(CTransform_t{{0,15,-10}, {0,0,0}});
    // ePlayer = Factory.createPlayer(CTransform_t{{0,0,10}});

    // Factory.createPlayer(Vector3f{0,0,0});
    // Factory.createPlayer(Vector3f{0,0,0});
    // Factory.createPlayer(CTransform_t{{10,0,5}, {0,0,0}, {4,4,4}});
    // Factory.createStaticMesh("assets/scene.gltf", {{},{},{.1,.1,.1}});

    loop();
}

void Game_t::loop() {
    constexpr uint32_t kFPS = 120;
    constexpr float deltatime = 1.0f / kFPS; // fixed delta for now

    Render.SetTargetFPS(kFPS);
    Input.DisableCursor();

    const CInput_t cameraControls = CreateFlyingCameraControls();
    ECS::Entityid_t camera = Factory.createFlyingCamera(CTransform_t{{0,25,-30}});

    Factory.createPhysicsPlane(CTransform_t{{0,0,0}});
    // these 4 are just 4 fun, to create edges around the platform
    // Factory.createPhysicsPlane(CTransform_t{{40,4,0}});
    // Factory.createPhysicsPlane(CTransform_t{{-40,4,0}});
    // Factory.createPhysicsPlane(CTransform_t{{0,4,40}});
    // Factory.createPhysicsPlane(CTransform_t{{0,4,-40}});

    // change x
    const float separation = 1.5f;
    Factory.createPhysicsBall(CTransform_t{{separation,26,0}});
    Factory.createPhysicsBall(CTransform_t{{-separation,28,0}});
    // change z
    Factory.createPhysicsBall(CTransform_t{{0,30,separation}});
    Factory.createPhysicsBall(CTransform_t{{0,32,-separation}});

    ECS::Entityid_t character = Factory.createCharacter(CTransform_t{{0,50,0}});
    CRigidbody_t& characterBody = EntMan.getComponent<CRigidbody_t>(character);

    while (Render.isAlive()) {
        if(Input.IsMouseButtonPressed(MouseButton_t::RIGHT)) {
            // Input.IsCursorHidden() ? Input.EnableCursor() : Input.HideCursor();
            Input.IsCursorHidden() ? Input.EnableCursor() : Input.DisableCursor();
        }

        if(Input.IsKeyPressed(Key_t::TAB)) {
            if(EntMan.hasAllComponents<CInput_t>(camera)) {
                EntMan.removeComponent<CInput_t>(camera);
            } else {
                EntMan.addComponent<CInput_t>(camera, cameraControls);
            }
        }

        if(Input.IsKeyDown(Key_t::KUP)) {
            std::cout << "KUP!\n";
            // ApplyCentralForce(characterBody, Vector3f_t{0,0,100});
            Translate(characterBody, Vector3f_t{0,0,5 * deltatime});
        }
        if(Input.IsKeyDown(Key_t::KDOWN)) {
            // ApplyCentralForce(characterBody, Vector3f_t{0,0,-100});
            Translate(characterBody, Vector3f_t{0,0,-5 * deltatime});
            std::cout << "KDOWN!\n";
        }
        if(Input.IsKeyDown(Key_t::KLEFT)) {
            // ApplyCentralForce(characterBody, Vector3f_t{100,0,0});
            Translate(characterBody, Vector3f_t{5 * deltatime,0,0});
            std::cout << "KLEFT!\n";
        }
        if(Input.IsKeyDown(Key_t::KRIGHT)) {
            // ApplyCentralForce(characterBody, Vector3f_t{-100,0,0});
            Translate(characterBody, Vector3f_t{-5 * deltatime,0,0});
            std::cout << "KRIGHT!\n";
        }
        
        Input.update(EntMan,deltatime);
        Physics.update(EntMan, deltatime);
        Render.update(EntMan);
    }
}
