#pragma once
#include "ecs/manager.hpp"
#include "game/sys/helpers/all.hpp"
#include "game/man/factory/factory.hpp"


struct Game_t {
    explicit Game_t();

private:
    void loop();

    ECS::EntityManager_t EntMan {};
    SRender_t Render { 1920, 1080 }; // 2304, 1296
    SInput_t Input { KeyBindings_t::RaylibBindings() };
    SPhysics_t Physics { Vector3f_t{ 0.0f, -9.81f, 0.0f } };
    EntityFactory_t Factory { EntMan };
};
