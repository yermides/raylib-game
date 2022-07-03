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
    
    void update(ECS::EntityManager_t& EntMan, const float deltatime);

    bool IsKeyDown(Key_t key) const;
    bool IsKeyUp(Key_t key) const;
    bool IsKeyPressed(Key_t key) const;
    bool IsKeyReleased(Key_t key) const;

    bool IsMouseButtonPressed(MouseButton_t button) const;
    bool IsMouseButtonDown(MouseButton_t button) const;
    bool IsMouseButtonReleased(MouseButton_t button) const;
    bool IsMouseButtonUp(MouseButton_t button) const;

    Vector2f_t GetMousePosition(void) const;
    Vector2f_t GetMouseDelta(void) const;

    void ShowCursor(void);
    void HideCursor(void);
    bool IsCursorHidden(void);
    void EnableCursor(void); // unlocks cursor
    void DisableCursor(void); // locks cursor
    bool IsCursorOnScreen(void);
private:
    int getKeyValue(Key_t key) const;
    int getMouseButtonValue(MouseButton_t button) const;

    // equivalence mapping provided by the keybindings
    std::unordered_map<int, int> keyboard   = {};
    std::unordered_map<int, int> mouse      = {};

    using Callback_t = std::function<bool(SInput_t*, Key_t)>;
    inline static const std::unordered_map<KeyState_t, Callback_t> checks = {
            { KeyState_t::UP,       [](SInput_t* Input, Key_t key) -> bool { return Input->IsKeyUp(key); } }
        ,   { KeyState_t::DOWN,     [](SInput_t* Input, Key_t key) -> bool { return Input->IsKeyDown(key); } }
        ,   { KeyState_t::PRESSED,  [](SInput_t* Input, Key_t key) -> bool { return Input->IsKeyPressed(key); } }
        ,   { KeyState_t::RELEASED, [](SInput_t* Input, Key_t key) -> bool { return Input->IsKeyReleased(key); } }
    };
};

CInput_t CreateFlyingCameraControls();
// namespace ComponentFunctions {}