#pragma once
#include <string_view>
#include "ecs/manager.hpp"
#include "helpers/vectors.hpp"
#include "game/cmp/helpers/all.hpp"

struct CTransform_t;

struct EntityFactory_t {
    #define EntityFactoryGeneralParams const CTransform_t& ptransform = {}

    explicit EntityFactory_t(ECS::EntityManager_t& pEntMan);
    ECS::Entityid_t createPlayer(const Vector3f& position);
    ECS::Entityid_t createPlayer(EntityFactoryGeneralParams);
    ECS::Entityid_t createCamera(const Vector3f& position);
    ECS::Entityid_t createStaticMesh(std::string_view filepath, EntityFactoryGeneralParams);

private:
    ECS::EntityManager_t& EntMan;
};
