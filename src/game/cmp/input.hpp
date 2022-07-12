#pragma once
#include <list>
#include <functional>
#include "helpers/keyboard.hpp"
#include "game/helpers/vector2.hpp"

struct InputAction_t {
    // TODO: change signature so it accepts deltatime as const float
    using Callback_t = std::function<void(ECS::EntityManager_t&, ECS::Entityid_t, const float deltatime)>; 

    Key_t requiredKey;
    KeyState_t requiredState;
    Callback_t callback;
};

struct MouseDeltaAction_t {
    using Callback_t = std::function<void(ECS::EntityManager_t&, ECS::Entityid_t, const Vector2f_t&)>; 
    
    Callback_t callback;
};

struct CInput_t {
    template<typename T>
    using Container_t = std::list<T>;

    Container_t<InputAction_t> actions {};
    Container_t<MouseDeltaAction_t> mouseDeltaActions {};
};
