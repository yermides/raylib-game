#pragma once
#include <string_view>
#include "ecs/manager.hpp"
#include "game/cmp/helpers/all.hpp"

struct EntityFactory_t {
    explicit EntityFactory_t(ECS::EntityManager_t& pEntMan);
    ECS::Entityid_t createPlayer(const CTransform_t& ptransform = {});
    ECS::Entityid_t createCamera(const CTransform_t& ptransform = {});
    ECS::Entityid_t createFlyingCamera(const CTransform_t& ptransform = {});
    ECS::Entityid_t createStaticMesh(std::string_view filepath, const CTransform_t& ptransform = {});

    // test for new physics components
    ECS::Entityid_t createPhysicsPlane(const CTransform_t& ptransform = {});
    ECS::Entityid_t createPhysicsBall(const CTransform_t& ptransform = {});

    // test for character controller
    ECS::Entityid_t createCharacter(const CTransform_t& ptransform = {});
private:
    ECS::EntityManager_t& EntMan;
};
