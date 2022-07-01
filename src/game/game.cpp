#include "game.hpp"
#include <iostream>
#include <imgui/imgui.h>
#include "game/cmp/helpers/all.hpp"
#include "helpers/includes/raylib.hpp"
#include "helpers/vector3.hpp"

Game_t::Game_t() {
    Vector3f_t v1 {1,1,1};
    Vector3f_t v2 {2,3,4};

    std::cout << "Hello from Game!\n";
    std::cout << ImGui::GetVersion() << "\n";
    Vector3f_t::print(v1 = v2);

    EntMan.connectOnContruct<CCamera_t, &SRender_t::setMainCamera>(Render);
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
    // const Vector3f_t offset = Vector3f_t{0,15,-20};
    // auto [cTransform, cCamera] = EntMan.getComponents<CTransform_t, CCamera_t>(eCamera);
    // CTransform_t& transform = EntMan.getComponent<CTransform_t>(ePlayer);

    ECS::Entityid_t camera = Factory.createFlyingCamera(CTransform_t{{0,15,-10}, {0,0,0}});

    while (Render.isAlive()) {
        if(Input.IsMouseButtonPressed(MouseButton_t::RIGHT)) {
            Input.IsCursorHidden() ? Input.EnableCursor() : Input.HideCursor();
        }

        if(Input.IsKeyPressed(Key_t::TAB)) {
            if(EntMan.hasAllComponents<CInput_t>(camera)) {
                EntMan.removeComponent<CInput_t>(camera);
            } else {
                EntMan.addComponent<CInput_t>(camera, CreateFlyingCameraControls());
            }
        }
        
        Input.update(EntMan);
        Physics.update(EntMan);
        Render.update(EntMan);
    }
}
