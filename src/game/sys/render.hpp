#pragma once
#include "ecs/manager.hpp"
#include "game/cmp/helpers/all.hpp"

enum RenderFlags_t {
        NONE            = 0 << 0
    ,   ANTIALIASING    = 1 << 0
    ,   RESIZABLE       = 1 << 1
};

struct SRender_t {
    explicit SRender_t(uint32_t width = 1600, uint32_t height = 900, RenderFlags_t flags = RenderFlags_t::NONE);
    ~SRender_t();

    void update(ECS::EntityManager_t& EntMan);
    bool isAlive(void) const;
    void drawGrid(int32_t divisions, float spacing);
    void drawGizmo();
    float getDeltatime();

    void SetTargetFPS(uint32_t fps);

    void setMainCamera(ECS::ComponentRegistry_t& registry, ECS::Entityid_t camera);
    void unloadModel(ECS::ComponentRegistry_t& registry, ECS::Entityid_t model);
private:
    static void updateOne(ECS::Entityid_t entity, CTransform_t& transform, CModelRenderer_t& model);
    void uploadCameraValues(ECS::EntityManager_t& EntMan, RL::Camera3D& camera);
    void drawEverything(ECS::EntityManager_t& EntMan);

    ECS::Entityid_t mainCamera {};
    RL::Camera3D rlcamera {};
};
