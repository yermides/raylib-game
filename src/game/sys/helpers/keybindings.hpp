#pragma once
#include <unordered_map>
#include "game/helpers/includes/raylib.hpp"
#include "game/cmp/helpers/keyboard.hpp"
// #include "keyboard.hpp"

// Key bindings configuration
struct KeyBindings_t {
    inline static KeyBindings_t RaylibBindings() {
        KeyBindings_t config {};
        config.equivalences = rlkeys;
        config.mouseequiv = rlmousebuttons;
        return config;
    }

    std::unordered_map<int, int> equivalences   = {};
    std::unordered_map<int, int> mouseequiv     = {};
private:
    explicit KeyBindings_t() = default;

    inline static const std::unordered_map<int, int> rlkeys = {
                { Key_t::EMPTY          , RL::KEY_NULL          }
            ,   { Key_t::A              , RL::KEY_A             }
            ,   { Key_t::B              , RL::KEY_B             }
            ,   { Key_t::C              , RL::KEY_C             }
            ,   { Key_t::D              , RL::KEY_D             }
            ,   { Key_t::E              , RL::KEY_E             }
            ,   { Key_t::F              , RL::KEY_F             }
            ,   { Key_t::G              , RL::KEY_G             }
            ,   { Key_t::H              , RL::KEY_H             }
            ,   { Key_t::I              , RL::KEY_I             }
            ,   { Key_t::J              , RL::KEY_J             }
            ,   { Key_t::K              , RL::KEY_K             }
            ,   { Key_t::L              , RL::KEY_L             }
            ,   { Key_t::M              , RL::KEY_M             }
            ,   { Key_t::N              , RL::KEY_N             }
            ,   { Key_t::O              , RL::KEY_O             }
            ,   { Key_t::P              , RL::KEY_P             }
            ,   { Key_t::Q              , RL::KEY_Q             }
            ,   { Key_t::R              , RL::KEY_R             }
            ,   { Key_t::S              , RL::KEY_S             }
            ,   { Key_t::T              , RL::KEY_T             }
            ,   { Key_t::U              , RL::KEY_U             }
            ,   { Key_t::V              , RL::KEY_V             }
            ,   { Key_t::W              , RL::KEY_W             }
            ,   { Key_t::X              , RL::KEY_X             }
            ,   { Key_t::Y              , RL::KEY_Y             }
            ,   { Key_t::Z              , RL::KEY_Z             }
            ,   { Key_t::SPACE          , RL::KEY_SPACE         }
            ,   { Key_t::ESCAPE         , RL::KEY_ESCAPE        }
            ,   { Key_t::ENTER          , RL::KEY_ENTER         }
            ,   { Key_t::TAB            , RL::KEY_TAB           }
            ,   { Key_t::KRIGHT         , RL::KEY_RIGHT         }
            ,   { Key_t::KLEFT          , RL::KEY_LEFT          }
            ,   { Key_t::KDOWN          , RL::KEY_DOWN          }
            ,   { Key_t::KUP            , RL::KEY_UP            }
            ,   { Key_t::LEFT_SHIFT     , RL::KEY_LEFT_SHIFT    }
            ,   { Key_t::LEFT_CONTROL   , RL::KEY_LEFT_CONTROL  }
            ,   { Key_t::LEFT_ALT       , RL::KEY_LEFT_ALT      }
            ,   { Key_t::RIGHT_SHIFT    , RL::KEY_RIGHT_SHIFT   }
            ,   { Key_t::RIGHT_CONTROL  , RL::KEY_RIGHT_CONTROL }
            ,   { Key_t::RIGHT_ALT      , RL::KEY_RIGHT_ALT     }
        };

    inline static const std::unordered_map<int, int> rlmousebuttons = {
            { MouseButton_t::LEFT       , RL::MOUSE_BUTTON_LEFT     }
        ,   { MouseButton_t::RIGHT      , RL::MOUSE_BUTTON_RIGHT    }
        ,   { MouseButton_t::MIDDLE     , RL::MOUSE_BUTTON_MIDDLE   }
        ,   { MouseButton_t::SIDE       , RL::MOUSE_BUTTON_SIDE     }
        ,   { MouseButton_t::EXTRA      , RL::MOUSE_BUTTON_EXTRA    }
        ,   { MouseButton_t::FORWARD    , RL::MOUSE_BUTTON_FORWARD  }
        ,   { MouseButton_t::BACK       , RL::MOUSE_BUTTON_BACK     }
    };
};
