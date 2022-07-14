#pragma once

struct CCharacterMovement_t {
    float fJumpForce { 100.0f }, fVelocity = { 15.0f };
    bool bCanJump { false };
};
