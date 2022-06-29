#include "input.hpp"
#include <iostream>

SInput_t::SInput_t(const KeyBindings_t& bindings) {
    keyboard = bindings.equivalences;
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
                Vector2f_t mouseDelta = getMouseDelta();
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

Vector2f_t SInput_t::getMouseDelta() const {
    RL::Vector2 mouseDelta = RL::GetMouseDelta();
    return { mouseDelta.x, mouseDelta.y };
}

// Private

int SInput_t::getKeyValue(Key_t key) const {
    return keyboard.at(key);
}

void SInput_t::updateOne(ECS::Entityid_t e, CInput_t& input) {
    // static std::unordered_map<int, std::function<bool()>> bindings = {
    //     { 0, }
    // }

    // for (auto& action : input.actions) {
    //     auto func = checks.at(action.requiredState);

    //     if(func(*this, action.requiredKey) && ) {

    //     }
    // }
}

// Anonymous functions

CInput_t CreateFlyingCameraControls() {
    CInput_t input {};

    {
        MouseDeltaAction_t action {
            [](ECS::EntityManager_t& EntMan, ECS::Entityid_t e, const Vector2f_t vector) {
                const float sensibility = 0.1f;
                CTransform_t& transform = EntMan.getComponent<CTransform_t>(e);
                transform.rotation.add_x(vector.get_y() * sensibility * -1.0f);
                transform.rotation.add_y(vector.get_x() * sensibility * -1.0f);
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