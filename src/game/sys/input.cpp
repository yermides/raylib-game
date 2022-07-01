#include "input.hpp"
#include <iostream>

SInput_t::SInput_t(const KeyBindings_t& bindings) {
    keyboard = bindings.equivalences;
    mouse = bindings.mouseequiv;
}

// TODO: clean this mess, it works but please...
void SInput_t::update(ECS::EntityManager_t& EntMan) {
    auto lambda = [this, &EntMan](auto e, CInput_t& input) {
        // Keyboard input actions
        for (auto& action : input.actions) {
            auto func = checks.at(action.requiredState);
            Key_t keycode = action.requiredKey;

            if(func(this, keycode) && action.callback) {
                action.callback(EntMan, e);
            }
        }

        // Mouse delta actions
        for (auto& action : input.mouseDeltaActions) {
            if(action.callback) {
                Vector2f_t mouseDelta = GetMouseDelta();
                action.callback(EntMan, e, mouseDelta);
            }
        }
    };

    EntMan.forAllMatching<CInput_t>(lambda);
}

bool SInput_t::IsKeyDown(Key_t key) const {
    int equivalenceKey = getKeyValue(key);
    return RL::IsKeyDown(equivalenceKey);
}

bool SInput_t::IsKeyUp(Key_t key) const {
    int equivalenceKey = getKeyValue(key);
    return RL::IsKeyUp(equivalenceKey);
}

bool SInput_t::IsKeyPressed(Key_t key) const {
    int equivalenceKey = getKeyValue(key);
    return RL::IsKeyPressed(equivalenceKey);
}

bool SInput_t::IsKeyReleased(Key_t key) const {
    int equivalenceKey = getKeyValue(key);
    return RL::IsKeyReleased(equivalenceKey);
}

bool SInput_t::IsMouseButtonPressed(MouseButton_t button) const {
    int equivalenceButton = getMouseButtonValue(button);
    return RL::IsMouseButtonPressed(equivalenceButton);
}

bool SInput_t::IsMouseButtonDown(MouseButton_t button) const {
    int equivalenceButton = getMouseButtonValue(button);
    return RL::IsMouseButtonDown(equivalenceButton);
}

bool SInput_t::IsMouseButtonReleased(MouseButton_t button) const {
    int equivalenceButton = getMouseButtonValue(button);
    return RL::IsMouseButtonReleased(equivalenceButton);
}

bool SInput_t::IsMouseButtonUp(MouseButton_t button) const {
    int equivalenceButton = getMouseButtonValue(button);
    return RL::IsMouseButtonUp(equivalenceButton);
}

Vector2f_t SInput_t::GetMousePosition(void) const {
    RL::Vector2 mousePosition = RL::GetMousePosition();
    return { mousePosition.x, mousePosition.y };
}

Vector2f_t SInput_t::GetMouseDelta(void) const {
    RL::Vector2 mouseDelta = RL::GetMouseDelta();
    return { mouseDelta.x, mouseDelta.y };
}

void SInput_t::ShowCursor(void) {
    RL::ShowCursor(); // It has to be paired with EnableCursor I believe, due to Raylib's weirdness
}

void SInput_t::HideCursor(void) {
    RL::HideCursor();
}

bool SInput_t::IsCursorHidden(void) {
    return RL::IsCursorHidden();
}

void SInput_t::EnableCursor(void) {
    RL::EnableCursor();
}

void SInput_t::DisableCursor(void) {
    RL::DisableCursor();
}

bool SInput_t::IsCursorOnScreen(void) {
    return RL::IsCursorOnScreen();
}

// Private

int SInput_t::getKeyValue(Key_t key) const {
    return keyboard.at(key);
}

int SInput_t::getMouseButtonValue(MouseButton_t button) const {
    return mouse.at(button);
}

// Anonymous functions

CInput_t CreateFlyingCameraControls() {
    CInput_t input {};

    {
        MouseDeltaAction_t action {
            [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e, const Vector2f_t vector) {
                const float sensibility = 0.15f;
                CTransform_t& transform = EntMan.getComponent<CTransform_t>(e);
                transform.rotation.add_x(vector.get_y() * sensibility * -1.0f);
                transform.rotation.add_y(vector.get_x() * sensibility * -1.0f);
                float clamped_pitch = std::clamp(transform.rotation.get_x(), -89.f, 89.f);
                transform.rotation.set_x(clamped_pitch);
            }
        };

        input.mouseDeltaActions.emplace_back(action);
    }
    {
        InputAction_t action { 
            Key_t::W
        ,   KeyState_t::DOWN
        ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                transform.position += transform.rotation.forward();
            } 
        };

        input.actions.emplace_back(action);
    }
    {
        InputAction_t action { 
            Key_t::A
        ,   KeyState_t::DOWN
        ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                transform.position -= transform.rotation.right();
            } 
        };

        input.actions.emplace_back(action);
    }
    {
        InputAction_t action { 
            Key_t::S
        ,   KeyState_t::DOWN
        ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                transform.position -= transform.rotation.forward();
            } 
        };

        input.actions.emplace_back(action);
    }
    {
        InputAction_t action { 
            Key_t::D
        ,   KeyState_t::DOWN
        ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                transform.position += transform.rotation.right();
            } 
        };

        input.actions.emplace_back(action);
    }
    {
        InputAction_t action { 
            Key_t::SPACE
        ,   KeyState_t::DOWN
        ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                transform.position += Vector3f_t{0,1,0};
            } 
        };

        input.actions.emplace_back(action);
    }
    {
        InputAction_t action { 
            Key_t::LEFT_CONTROL
        ,   KeyState_t::DOWN
        ,   [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e) {
                auto [transform, camera] = EntMan.getComponents<CTransform_t, CCamera_t>(e);
                transform.position -= Vector3f_t{0,1,0};
            } 
        };

        input.actions.emplace_back(action);
    }

    return input;
}