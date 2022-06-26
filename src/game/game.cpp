#include "game.hpp"
#include "game/cmp/helpers/all.hpp"
#include "helpers/includes/raylib.hpp"
#include <iostream>

Game_t::Game_t() {
    std::cout << "Hello from Game!\n";

    EntMan.connectOnContruct<CCamera_t, &SRender_t::setMainCamera>(Render);
    EntMan.connectOnRemove<CModelRenderer_t, &SRender_t::unloadModel>(Render);

    eCamera = Factory.createCamera(Vector3f{0,15,-10});
    ePlayer = Factory.createPlayer(CTransform_t{{0,0,10}});

    // Factory.createPlayer(Vector3f{0,0,0});
    // Factory.createPlayer(Vector3f{0,0,0});
    // Factory.createPlayer(CTransform_t{{10,0,5}, {0,0,0}, {4,4,4}});
    // Factory.createStaticMesh("assets/scene.gltf", {{},{},{.1,.1,.1}});

    loop();
}

void Game_t::loop() {
    const Vector3f offset = Vector3f{0,15,-20};

    while (Render.isAlive()) {
        Input.update(EntMan);

        auto [cTransform, cCamera] = EntMan.getComponents<CTransform_t, CCamera_t>(eCamera);
        CTransform_t& transform = EntMan.getComponent<CTransform_t>(ePlayer);
        cTransform.position = transform.position + offset;
        cCamera.target = transform.position;

        Render.update(EntMan);

        // if(Input.IsKeyPressed(Key_t::A)) {
        //     std::cout <<"A";
        // }
    }
}