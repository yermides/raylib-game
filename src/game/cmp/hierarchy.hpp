#pragma once
#include <list>
#include "ecs/manager.hpp"

struct CHierarchy_t {
    // using EntityList_t = std::list<ECS::Entityid_t*>;
    // ECS::Entityid_t* parent {};
    // EntityList_t children {};

    std::size_t children    {};
    ECS::Entityid_t first   { ECS::NullEntity };
    ECS::Entityid_t prev    { ECS::NullEntity };
    ECS::Entityid_t next    { ECS::NullEntity };
    ECS::Entityid_t parent  { ECS::NullEntity };
};
