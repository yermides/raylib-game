#include "render.hpp"
#include <iostream>
#include <imgui/imgui.h>
#include "helpers/includes/raylib.hpp"
#include "helpers/adapters/vector.hpp"

SRender_t::SRender_t(uint32_t width, uint32_t height, RenderFlags_t flags) {
    uint32_t configFlags = 0
    |   RL::FLAG_MSAA_4X_HINT
    |   RL::FLAG_VSYNC_HINT
    ;

    RL::SetConfigFlags(configFlags);
    RL::InitWindow(width, height, "My Raylib Game");

    uint32_t stateFlags = 0
    // |   RL::FLAG_WINDOW_UNDECORATED
    |   RL::FLAG_WINDOW_RESIZABLE
    |   RL::FLAG_WINDOW_RESIZABLE
    ;

    RL::SetWindowState(stateFlags);
    RL::SetTargetFPS(120); // WARNING, puts to sleep the ENTIRE main thread TODO: remove from here

    RL::rlImGuiSetup(true);
}

SRender_t::~SRender_t() {
    RL::rlImGuiShutdown();
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
    RL::DrawCylinderEx({0,0,0}, {0,5,0}, 0.3, 0.3, 12, RL::GREEN);
    RL::DrawCylinderEx({0,5,0}, {0,6,0}, .5, 0, 12, RL::GREEN);

    RL::DrawCylinderEx({0,0,0}, {5,0,0}, 0.3, 0.3, 12, RL::RED);
    RL::DrawCylinderEx({5,0,0}, {6,0,0}, .5, 0, 12, RL::RED);

    RL::DrawCylinderEx({0,0,0}, {0,0,5}, 0.3, 0.3, 12, RL::BLUE);
    RL::DrawCylinderEx({0,0,5}, {0,0,6}, .5, 0, 12, RL::BLUE);

    // RL::DrawTriangle3D({1.2,5,0},{-1.2,5,0},{0,6.2,0}, RL::GREEN);

}

float SRender_t::getDeltatime() {
    return RL::GetFrameTime();
}

void SRender_t::SetTargetFPS(uint32_t fps) {
    RL::SetTargetFPS(fps);
}

void SRender_t::SetMainCamera(ECS::Entityid_t camera) {
    mainCamera = camera;
}

void SRender_t::setMainCamera(ECS::ComponentRegistry_t& registry, ECS::Entityid_t camera) {
    const ECS::EntityManager_t& EntMan = registry.ctx().at<ECS::EntityManager_t>();

    // set main camera only if there was none prior
    if(!EntMan.isValid(mainCamera)) {
        // SetMainCamera(camera);
        mainCamera = camera;
        std::cout << "camera set!\n";
    }
}

void SRender_t::unloadModel(ECS::ComponentRegistry_t& registry, ECS::Entityid_t e) {
    CModelRenderer_t& model = registry.get<CModelRenderer_t>(e);
    RL::UnloadModel(model.model);
}

// private 

void SRender_t::updateOne(ECS::Entityid_t entity, CTransform_t& transform, CModelRenderer_t& model) {
    // model.model.transform
    // RL::DrawModelEx
    RL::DrawModel(model.model, transform.position, transform.scale.get_x(), RL::WHITE); // TODO: improve
    // std::cout << "updating one tr & model\n";
}

void SRender_t::uploadCameraValues(ECS::EntityManager_t& EntMan, RL::Camera3D& camera) {
    // check if there is a valid camera entity
    if(CCamera_t* cmp_cam = EntMan.tryGetComponent<CCamera_t>(mainCamera); cmp_cam != nullptr) {
        CTransform_t& transform = EntMan.getComponent<CTransform_t>(mainCamera);
        camera.position = transform.position;
        // camera.target = cmp_cam->target; // TODO: put this in the camera system
        camera.target = transform.position + transform.rotation.forward();
        
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
    SCPhysicsDrawingContext_t& context = EntMan.getSingletonComponent<SCPhysicsDrawingContext_t>();

    RL::BeginDrawing();
    {
        RL::rlImGuiBegin();
        RL::ClearBackground(RL::RAYWHITE);

        RL::BeginMode3D(rlcamera);
        {
            EntMan.forAllMatching<CTransform_t, CModelRenderer_t>(updateOne);
            drawGizmo();

            if(context.lines) {
                const std::vector<DebugLine3D_t>& lines = *(context.lines);

                for(auto& line : lines) {
                    Vector3f_t from = line.from;
                    Vector3f_t to = line.to;

                    RL::DrawLine3D(from, to, RL::VIOLET);
                }
            }
            // RL::DrawMesh(plane, RL::Material{}, RL::Matrix{});
            RL::DrawGrid(20, 10.0f);
        }
        RL::EndMode3D();

        RL::DrawText("Lucas Mataix Garrigós", 1920 - 276, 1080 - 28, 24, RL::GRAY);
        RL::DrawFPS(10, 10);

        // ImGui::ShowDemoWindow();
        RL::rlImGuiEnd();
    }
    RL::EndDrawing();
}
