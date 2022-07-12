#pragma once

struct CCollisionable_t;

enum class CollisionEventType_t {
        VOID_COLLISION // default type to initialize

    ,   BODY_WITH_BODY

    ,   TRIGGER_WITH_BODY
    ,   BODY_WITH_TRIGGER = TRIGGER_WITH_BODY

    ,   CHARACTER_WITH_BODY
    ,   BODY_WITH_CHARACTER = CHARACTER_WITH_BODY

    ,   TRIGGER_WITH_TRIGGER
};

// The breakdown of the contact information of the collision
struct CollisionEvent_t {
    CollisionEventType_t type { CollisionEventType_t::VOID_COLLISION };
    ECS::EntityManager_t& EntMan;
    ECS::Entityid_t selfEntity {}, otherEntity {};

    // TODO: add more collision information like the contact points, impulse applied, etc (manifold info)
};
