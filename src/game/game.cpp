#include "game.hpp"
#include "game/cmp/helpers/all.hpp"
#include "game/helpers/includes/raylib.hpp"
#include "game/helpers/vector3.hpp"
#include "game/helpers/logger.hpp"
#include <enet/enet.h>

Game_t::Game_t() {
    LOG_INIT();


    EntMan.connectOnContruct<CCamera_t, &SRender_t::setMainCamera>(Render);
    EntMan.connectOnContruct<CBoxCollider_t, &SPhysics_t::registerAddColliderToWorld>(Physics);
    EntMan.connectOnContruct<CSphereCollider_t, &SPhysics_t::registerAddColliderToWorld>(Physics);
    EntMan.connectOnContruct<CCapsuleCollider_t, &SPhysics_t::registerAddColliderToWorld>(Physics);
    EntMan.connectOnContruct<CRigidbody_t, &SPhysics_t::registerAddRigidbodyToWorld>(Physics);
    EntMan.connectOnContruct<CTriggerVolume_t, &SPhysics_t::registerAddTriggerToWorld>(Physics);
    // EntMan.connectOnContruct<CCharacterController_t, &SPhysics_t::registerAddCharacterToWorld>(Physics); // TODO:
    EntMan.connectOnRemove<CModelRenderer_t, &SRender_t::unloadModel>(Render);
    EntMan.connectOnRemove<CRigidbody_t, &SPhysics_t::removeAndDeleteBodyFromWorld>(Physics);

}

void Game_t::loop() {
    constexpr uint32_t kFPS = 60;
    constexpr float deltatime = 1.0f / kFPS; // fixed delta for now

    enet_initialize();
    enet_deinitialize();

    Render.SetTargetFPS(kFPS);
    Physics.setGravity(Vector3f_t{0,-30,0});

    // const CInput_t cameraControls = CreateFlyingCameraControls();
    ECS::Entityid_t camera = Factory.createCamera(CTransform_t{{0,10,-10},{-20,0,0}});

    Factory.createPhysicsPlane(CTransform_t{{0,0,0}});
    // ECS::Entityid_t camera = Factory.createFlyingCamera(CTransform_t{{0,25,-30}});
    // ECS::Entityid_t camera = Factory.createCamera(CTransform_t{{0,25,-30},{-45,180,0}});

    {
        const float separation = 4.5f;
        Factory.createPhysicsBall(CTransform_t{{separation,26,0}});
        // Factory.createPhysicsBall(CTransform_t{{-separation,28,0}});
        // Factory.createPhysicsBall(CTransform_t{{0,30,separation}});
        // Factory.createPhysicsBall(CTransform_t{{0,32,-separation}});
    }

    ECS::Entityid_t character = Factory.createCharacter(CTransform_t{{0,8,0}});
    CRigidbody_t& characterBody = EntMan.getComponent<CRigidbody_t>(character);

    auto& charTr = EntMan.getComponent<CTransform_t>(character).position;
    auto& camTr = EntMan.getComponent<CTransform_t>(camera).position;
    auto& camTrans = EntMan.getComponent<CTransform_t>(camera);
    auto& charTrans = EntMan.getComponent<CTransform_t>(character);

    Factory.createTrigger(CTransform_t{{0,15,0}});

    const Vector3f_t distanceToCamera = camTr - charTr;
    Vector3f_t dest = (charTr + (GetBackVector(camTrans) * 30.0f)) + Vector3f_t{0,10,0};
    camTrans.position = dest;

    while (Render.isAlive()) {
        dest = (charTr + (GetBackVector(camTrans) * 30.0f)) + Vector3f_t{0,10,0};
        camTr = Vector3f_t::lerp(camTr, dest, 1.0f * deltatime);
        
        Input.update(EntMan,deltatime);
        Physics.update(EntMan, deltatime);
        Render.update(EntMan);
    }
}

#ifdef __MINGW32__
    #include <mingw-std-threads/mingw.thread.h>
    #include <mingw-std-threads/mingw.mutex.h>
#else
    #include <thread>
    #include <mutex>
#endif

void Game_t::loop_multithread() {
    // this will remained paused for a while

    constexpr float kFPS = 60.0f;
    constexpr float kFixedDelta = 1.0f / kFPS;
    Render.SetTargetFPS(kFPS);
    ECS::Entityid_t camera = Factory.createFlyingCamera(CTransform_t{{0,25,-30}});

    while (Render.isAlive()) {
        Input.update(EntMan, kFixedDelta);
        // Physics.update(EntMan, kFixedDelta);
        Render.update(EntMan);
    }
}

// constexpr float vel = 15.0f;

// if(Input.IsKeyDown(Key_t::A)) {
//     camTrans.rotation.add_y((vel * 3.0f * deltatime));
// }

// if(Input.IsKeyDown(Key_t::D)) {
//     camTrans.rotation.add_y(-(vel * 3.0f * deltatime));
// }

// if(Input.IsKeyDown(Key_t::KUP)) {
//     Vector3f_t forward = GetForwardVector(camTrans);
//     forward.set_y(0);
//     Translate(characterBody, forward * vel * deltatime);
// }

// if(Input.IsKeyDown(Key_t::KDOWN)) {
//     Vector3f_t back = GetBackVector(camTrans);
//     back.set_y(0);
//     Translate(characterBody, back * vel * deltatime);
// }

// if(Input.IsKeyDown(Key_t::KLEFT)) {
//     Vector3f_t left = GetLeftVector(camTrans);
//     left.set_y(0);
//     Translate(characterBody, left * vel * deltatime);
// }

// if(Input.IsKeyDown(Key_t::KRIGHT)) {
//     Vector3f_t right = GetRightVector(camTrans);
//     right.set_y(0);
//     Translate(characterBody, right * vel * deltatime);
// }

// if(Input.IsKeyPressed(Key_t::SPACE)) {
//     // SetLinearVelocity(characterBody, Vector3f_t{0,0,0});
//     ApplyCentralImpulse(characterBody, Vector3f_t{0,300,0});
// }