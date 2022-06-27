#include "render.hpp"
#include "helpers/includes/raylib.hpp"
#include "helpers/adapters/vector.hpp"
#include <iostream>

SRender_t::SRender_t(uint32_t width, uint32_t height, RenderFlags_t flags) {
    RL::SetConfigFlags(RL::FLAG_MSAA_4X_HINT | RL::FLAG_VSYNC_HINT);
    RL::InitWindow(width, height, "My Raylib Game");
    // RL::SetWindowState(RL::FLAG_WINDOW_RESIZABLE);
    RL::SetTargetFPS(60); // WARNING, puts to sleep the ENTIRE main thread TODO: remove from here

//     if(flags & RenderFlags_t::ANTIALIASING) {

// }
}

SRender_t::~SRender_t() {
    RL::CloseWindow();
}

void SRender_t::update(ECS::EntityManager_t& EntMan) {
    uploadCameraValues(EntMan, rlcamera);
    drawEverything(EntMan);
}

bool SRender_t::isAlive(void) const {
    return !RL::WindowShouldClose();
}

void SRender_t::drawGrid(int32_t divisions, float spacing) {
    RL::DrawGrid(divisions, spacing);
}

void SRender_t::drawGizmo() {
    const RL::Vector3 start = {0,0,0};
    RL::DrawCylinderEx(start, {0,5,0}, 0.3, 0.3, 12, RL::GREEN);
    // RL::DrawCylinderWiresEx(start, {0,5,0}, 0.3, 0.3, 12, RL::WHITE);
    RL::DrawCylinderEx(start, {5,0,0}, 0.3, 0.3, 12, RL::RED);
    // RL::DrawCylinderWiresEx(start, {5,0,0}, 0.3, 0.3, 12, RL::WHITE);
    RL::DrawCylinderEx(start, {0,0,5}, 0.3, 0.3, 12, RL::BLUE);
    // RL::DrawCylinderWiresEx(start, {0,0,5}, 0.3, 0.3, 12, RL::WHITE);
}

float SRender_t::getDeltatime() {
    return RL::GetFrameTime();
}

void SRender_t::setMainCamera(ECS::ComponentRegistry_t& registry, ECS::Entityid_t camera) {
    mainCamera = camera;
    std::cout << "camera set!\n";
}

void SRender_t::unloadModel(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    CModelRenderer_t& model = registry.get<CModelRenderer_t>(e);
    RL::UnloadModel(model.model);
}

// private 

void SRender_t::updateOne(ECS::Entityid_t entity, CTransform_t& transform, CModelRenderer_t& model) {
    // model.model.transform
    // RL::DrawModelEx
    RL::DrawModel(model.model, transform.position, transform.scale.get_x(), RL::WHITE); // TODO:
    // std::cout << "updating one tr & model\n";
}

void SRender_t::uploadCameraValues(ECS::EntityManager_t& EntMan, RL::Camera3D& camera) {
    // check if there is a valid camera entity
    if(CCamera_t* cmp_cam = EntMan.tryGetComponent<CCamera_t>(mainCamera); cmp_cam != nullptr) {
        CTransform_t& transform = EntMan.getComponent<CTransform_t>(mainCamera);
        camera.position = transform.position;
        camera.target = cmp_cam->target; // TODO:
        camera.up = cmp_cam->up;
        camera.fovy = cmp_cam->fovy;
        camera.projection = RL::CAMERA_PERSPECTIVE;
    } else { // if no camera component in scene, use these default values
        camera.position = { 50.0f, 50.0f, 50.0f }; 
        camera.target = { 0.0f, 0.0f, 0.0f };
        camera.up = { 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = RL::CAMERA_PERSPECTIVE;
    }

    RL::UpdateCamera(&camera);
}

void SRender_t::drawEverything(ECS::EntityManager_t& EntMan) {
    RL::BeginDrawing();
        // RL::ClearBackground(RL::BLANK);
        RL::ClearBackground(RL::RAYWHITE);
        RL::BeginMode3D(rlcamera);
            EntMan.forAllMatching<CTransform_t, CModelRenderer_t>(updateOne);
            drawGizmo();
            // RL::DrawMesh(plane, RL::Material{}, RL::Matrix{});
            RL::DrawGrid(20, 10.0f);
        RL::EndMode3D();
        RL::DrawText("(c) Lucas Mataix Garrigós", 1600 - 200, 900 - 20, 10, RL::GRAY);
        RL::DrawFPS(10, 10);
    RL::EndDrawing();
}