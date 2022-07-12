#pragma once
#include <functional>
#include "game/helpers/types.hpp"
#include "helpers/collision_filtering.hpp"
#include "helpers/collision_event.hpp"

// The collisionable components saves the information about collision responses that should happen
// affecting entities and their components when collision objects (namely rigidbodies) collide

struct CCollisionable_t {
    using Callback_t = std::function<void(const CollisionEvent_t& collision)>;

    struct CallbackCollection_t {
        Callback_t onEventEnter, onEventStay, onEventExit;
    };

    Hashmap_t<CollisionEventType_t, CallbackCollection_t> callbacks;

    CollisionLayer_t    layer { CollisionLayer_t::AllFilter }; // currently not used, it may correspond to the rigidbody component really
    CollisionableType_t type  { CollisionableType_t::Default }; // type of object for the collision
    
    // Hashmap_t<CollisionEventType_t, Callback_t> callbacks;
    // callbacks should be cached in the global functions and activated in the internalTickCallback
    // also, to serialize them, there should be a cache in the physics that correlates ids and function pointers
};




// So, there are two ways to implement collision response callbacks (and I'll probably go for 1st)

// 1st is to use a map of collision types (enum) and callbacks, and set them as
// responses[CollisionEventType_t] = callback (std::function or anything)

// 2nd one is to create those callbacks independently, not using a type but calling the specific callback when needed
// example std::function<void(Args...)> 

// 3rd one: have it so, based on a pair of types, collision response is actually stored in the physics system
// and yeah, I'd only consider this one for small projects where triggers and physical colliders differentiation
//  wouldn't matter

// Just some stuff I had planned for the second approach
// bool bReceivesHitEvents {}; // TODO:
// std::function<void()> onBeginOverlapResponse, duringOverlapResponse, onEndOverlapResponse; // TODO: signature and usage
// std::function<void()> onHitResponse; // TODO:
