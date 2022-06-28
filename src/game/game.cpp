#include "game.hpp"
#include "game/cmp/helpers/all.hpp"
#include "helpers/includes/raylib.hpp"
#include <iostream>

#include "helpers/vector3.hpp"

Game_t::Game_t() {
    Vector3f_t v1 {1,1,1};
    Vector3f_t v2 {2,3,4};

    std::cout << "Hello from Game!\n";
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

    // glm::lookAt({0,0,0});

void Game_t::loop() {
    // const Vector3f_t offset = Vector3f_t{0,15,-20};
    // auto [cTransform, cCamera] = EntMan.getComponents<CTransform_t, CCamera_t>(eCamera);
    // CTransform_t& transform = EntMan.getComponent<CTransform_t>(ePlayer);

    Factory.createFlyingCamera(CTransform_t{{0,15,-10}, {0,0,0}});

    while (Render.isAlive()) {
        Input.update(EntMan);
        Render.update(EntMan);
    }
}


/*
        // cTransform.position = transform.position + offset;
        // cCamera.target = transform.position;

        cTransform.rotation += {0,1,0};
        // cCamera.target = cTransform.rotation.forward(); 

        if(Input.IsKeyDown(Key_t::W)) {
            cTransform.position += cTransform.rotation.forward();
        }

        if(Input.IsKeyDown(Key_t::S)) {
            cTransform.position -= cTransform.rotation.forward();
        }

        if(Input.IsKeyDown(Key_t::A)) {
            cTransform.position -= cTransform.rotation.right();
        }

        if(Input.IsKeyDown(Key_t::D)) {
            cTransform.position += cTransform.rotation.right();
        }

        if(Input.IsKeyDown(Key_t::SPACE)) {
            cTransform.position += cTransform.rotation.up();
        }

        if(Input.IsKeyDown(Key_t::TAB)) {
            cTransform.position -= cTransform.rotation.up();
        }

        // cCamera.target = {0,0,0};
        // cCamera.target = cTransform.position + cTransform.rotation.forward();
*/