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
