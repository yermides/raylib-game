#pragma once
#include <unordered_map>
#include <functional>
#include "ecs/manager.hpp"
#include "game/cmp/helpers/all.hpp"
#include "helpers/keybindings.hpp"
#include "helpers/includes/raylib.hpp"
#include "helpers/vector2.hpp"

struct SInput_t {
    explicit SInput_t(const KeyBindings_t& bindings);

    void update(ECS::EntityManager_t& EntMan);

    bool IsKeyDown(Key_t key) const;
    bool IsKeyUp(Key_t key) const;
    bool IsKeyPressed(Key_t key) const;
    bool IsKeyReleased(Key_t key) const;
    Vector2f_t getMouseDelta() const;
private:
    int getKeyValue(Key_t key) const;
    static void updateOne(ECS::Entityid_t e, CInput_t& input);

    // equivalence mapping provided by the keybindings
    std::unordered_map<int, int> keyboard = {};

    // using callback = void(*)(SInput_t&, Key_t);
    using Callback_t = std::function<bool(SInput_t*, Key_t)>;

    inline static const std::unordered_map<KeyState_t, Callback_t> checks = {
            { KeyState_t::UP,       [](SInput_t* Input, Key_t key) -> bool { return Input->IsKeyUp(key); } }
        ,   { KeyState_t::DOWN,     [](SInput_t* Input, Key_t key) -> bool { return Input->IsKeyDown(key); } }
        ,   { KeyState_t::PRESSED,  [](SInput_t* Input, Key_t key) -> bool { return Input->IsKeyPressed(key); } }
        ,   { KeyState_t::RELEASED, [](SInput_t* Input, Key_t key) -> bool { return Input->IsKeyReleased(key); } }
    };
};
