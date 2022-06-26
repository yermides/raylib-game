#pragma once
#include <list>
#include "ecs/manager.hpp"

struct CHierarchy_t {
    using EntityList_t = std::list<ECS::Entityid_t*>;

    ECS::Entityid_t* parent {};
    EntityList_t children {};
};
