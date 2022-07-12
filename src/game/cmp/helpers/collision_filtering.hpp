#pragma once

#ifndef BIT
    #define BITS_NONE 0
    #define BIT(X) 1 << X
    #define BITS_ALL -1
#endif

// These mimic Bullet's collision masks
// By default, if called addRigidbody with no mask and flags, it'll use DefaultFilter and StaticFilter
// The others up to character filter are defined but never used by bullet, so it doesn't even mater they're predefined
// they are just a hint for the user if he wants to define custom collision filtering for those types

// These are the "layers" that will constitude the collision "matrix"
// We can, for example, tell bullet we don't want collisions 
// between kinematics and sensors to ever be added (as a global concept)
// or enable collisions between kinematic && kinematic, or kinematic && static
// Once set, the filtering is handled automatically
// and we'll just care about collision types
enum class CollisionLayer_t {
        NoFilter         = BITS_NONE
    ,   DefaultFilter    = BIT( 0)
    ,   StaticFilter     = BIT( 1)
    // These four below are also bullet defined but not used internally by bullet
    ,   KinematicFilter  = BIT( 2)
    ,   DebrisFilter     = BIT( 3)
    ,   SensorTrigger    = BIT( 4)
    ,   CharacterFilter  = BIT( 5)
    // From here, we have all the space to create our custom masks
    ,   UserDefinedBit6  = BIT( 6)
    ,   UserDefinedBit7  = BIT( 7)
    ,   UserDefinedBit8  = BIT( 8)
    ,   UserDefinedBit9  = BIT( 9)
    ,   UserDefinedBit10 = BIT(10)
    ,   UserDefinedBit11 = BIT(11)
    ,   UserDefinedBit12 = BIT(12)
    ,   UserDefinedBit13 = BIT(13)
    ,   UserDefinedBit14 = BIT(14)
    ,   UserDefinedBit15 = BIT(15)
    ,   UserDefinedBit16 = BIT(16)
    ,   UserDefinedBit17 = BIT(17)
    ,   UserDefinedBit18 = BIT(18)
    ,   UserDefinedBit19 = BIT(19)
    ,   UserDefinedBit20 = BIT(20)
    ,   UserDefinedBit21 = BIT(21)
    ,   UserDefinedBit22 = BIT(22)
    ,   UserDefinedBit23 = BIT(23)
    ,   UserDefinedBit24 = BIT(24)
    ,   UserDefinedBit25 = BIT(25)
    ,   UserDefinedBit26 = BIT(26)
    ,   UserDefinedBit27 = BIT(27)
    ,   UserDefinedBit28 = BIT(28)
    ,   UserDefinedBit29 = BIT(29)
    ,   UserDefinedBit30 = BIT(30)
    ,   AllFilter        = BITS_ALL // it is bit in place 31
    // rigidbodies, ghost objects, etc
};

// These are the "tags", should be user defined really
enum class CollisionableType_t {
        Default
    ,   Player
    ,   Enemy
    ,   Platform
};

// This is to understand bullet's logic when it comes to filtering collisions
// The whole matrix can be constructed using these carefully
// with some hashmap of layerid and flags they discriminate against or something

// bool isDynamic = !(body->isStaticObject() || body->isKinematicObject());
// int collisionFilterGroup = isDynamic ? int(btBroadphaseProxy::DefaultFilter) : int(btBroadphaseProxy::StaticFilter);
// int collisionFilterMask = isDynamic ? int(btBroadphaseProxy::AllFilter) : int(btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter);
// addCollisionObject(body, collisionFilterGroup, collisionFilterMask);

// bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
// collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
// return collides;

// collisionFilterGroup = 0b00000000000000000000000000000001 (default =  1) else = 0b00000000000000000000000000000010)
// collisionFilterMask =  0b11111111111111111111111111111111 (default = -1) else = 0b11111111111111111111111111111101)
// collides = true (else false)
