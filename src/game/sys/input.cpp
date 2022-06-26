#include "input.hpp"
#include <iostream>

SInput_t::SInput_t(const KeyBindings_t& bindings) {
    keyboard = bindings.equivalences;
}

// TODO: clean this mess, it works but please...
void SInput_t::update(ECS::EntityManager_t& EntMan) {
    auto lambda = [this, &EntMan](auto e, CInput_t& input) {

        // std::cout << "lol\n";

        for (auto& action : input.actions) {
            auto func = checks.at(action.requiredState);
            Key_t keycode = action.requiredKey;

            // std::cout << keycode + "\n";

            if(func(this, keycode) && action.callback) {
                action.callback(EntMan, e);
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
