#pragma once
// #include <entt/entt.hpp>
#include <entt/entity/registry.hpp>

namespace ECS {
    using NullEntityType_t = entt::null_t;
    using Entityid_t = entt::entity;
    using ComponentRegistry_t = entt::registry;
    inline constexpr NullEntityType_t NullEntity = entt::null;

    struct EntityManager_t {
        explicit EntityManager_t(std::size_t size = 50) {
            m_singlecmpsStorage = m_singlecmpsRegistry.create();
            m_componentsRegistry.reserve(size);
            m_componentsRegistry.ctx().emplace<EntityManager_t&>(*this);
        };

        EntityManager_t(EntityManager_t const&)             = delete;
        EntityManager_t(EntityManager_t&&)                  = delete;
        EntityManager_t& operator=(EntityManager_t const&)  = delete;
        EntityManager_t& operator=(EntityManager_t&&)       = delete;

        inline Entityid_t createEntity(void) {
            return m_componentsRegistry.create();
        }

        inline bool isValid(Entityid_t e) const {
            return m_componentsRegistry.valid(e);
        }

        inline void destroyEntity(Entityid_t e) {
            m_componentsRegistry.destroy(e);
        }

        inline void destroyAllEntities(void) {
            m_componentsRegistry.clear();
        }

        template <typename Component_t>
        constexpr bool hasComponent(Entityid_t e) {
            return m_componentsRegistry.all_of<Component_t>(e);
        }

        template <typename... Args_t>
        constexpr bool hasAllComponents(Entityid_t e) {
            return m_componentsRegistry.all_of<Args_t...>(e);
        }

        template <typename... Args_t>
        constexpr bool hasAnyComponents(Entityid_t e) {
            return m_componentsRegistry.any_of<Args_t...>(e);
        }

        template <typename Component_t, typename... Args_t>
        constexpr Component_t& addComponent(Entityid_t e, Args_t&&... args) {
            return m_componentsRegistry.get_or_emplace<Component_t>(e, args...);
        }

        template <typename Component_t, typename... Args_t>
        constexpr const Component_t& addComponent(Entityid_t e, Args_t&&... args) const {
            return m_componentsRegistry.get_or_emplace<Component_t>(e, args...);
        }

        template <typename Component_t>
        constexpr Component_t& getComponent(Entityid_t e) {
            return m_componentsRegistry.get<Component_t>(e);
        }

        template <typename Component_t>
        constexpr const Component_t& getComponent(Entityid_t e) const {
            return m_componentsRegistry.get<Component_t>(e);
        }

        // gets a tuple of references to the queried components, capture in a structure binding pls
        template <typename... Args_t>
        constexpr auto getComponents(Entityid_t e) {
            return m_componentsRegistry.get<Args_t...>(e);
        }

        template <typename... Args_t>
        constexpr const auto getComponents(Entityid_t e) const {
            return m_componentsRegistry.get<Args_t...>(e);
        }

        template <typename Component_t>
        constexpr Component_t* tryGetComponent(Entityid_t e) {
            return m_componentsRegistry.try_get<Component_t>(e);
        }

        template <typename Component_t>
        constexpr const Component_t* tryGetComponent(Entityid_t e) const {
            return m_componentsRegistry.try_get<Component_t>(e);
        }

        template <typename Component_t>
        constexpr Entityid_t getEntity(Component_t& component) {
            return entt::to_entity(m_componentsRegistry, component);
        }

        template <typename Component_t>
        constexpr void removeComponent(Entityid_t e) {
            m_componentsRegistry.remove<Component_t>(e);
        }

        template <typename Func_t>
        constexpr void forAll(Func_t func) {
            m_componentsRegistry.each(func);
        }

        template <typename... Args_t, typename Func_t>
        constexpr void forAllMatching(Func_t&& func) {
            m_componentsRegistry.view<Args_t...>().each(func);
        }

        template <typename SingletonComponent_t>
        constexpr SingletonComponent_t& getSingletonComponent(void) {
            return m_singlecmpsRegistry.get_or_emplace<SingletonComponent_t>(m_singlecmpsStorage);
        }

        template <typename SingletonComponent_t>
        constexpr const SingletonComponent_t& getSingletonComponent(void) const {
            return m_singlecmpsRegistry.get_or_emplace<SingletonComponent_t>(m_singlecmpsStorage);
        }

        // on component create callbacks

        template <typename Component_t, auto Candidate_t>
        constexpr void connectOnContruct() {
            m_componentsRegistry.on_construct<Component_t>().template connect<Candidate_t>(); //.connect<Func_t>();
        }

        template <typename Component_t, auto Candidate_t, typename Instance_t>
        constexpr void connectOnContruct(Instance_t&& instance) {
            m_componentsRegistry.on_construct<Component_t>().template connect<Candidate_t>(instance); //.connect<Func_t>();
        }

        template <typename Component_t, auto Candidate_t>
        constexpr void disconnectOnContruct() {
            m_componentsRegistry.on_construct<Component_t>().template connect<Candidate_t>(); //.connect<Func_t>();
        }

        template <typename Component_t, auto Candidate_t, typename Instance_t>
        constexpr void disconnectOnContruct(Instance_t&& instance) {
            m_componentsRegistry.on_construct<Component_t>().template disconnect<Candidate_t>(instance); //.connect<Func_t>();
        }

        // on component updated callback

        template <typename Component_t, auto Candidate_t>
        constexpr void connectOnUpdate() {
            m_componentsRegistry.on_update<Component_t>().template connect<Candidate_t>();
        }

        template <typename Component_t, auto Candidate_t, typename Instance_t>
        constexpr void connectOnUpdate(Instance_t&& instance) {
            m_componentsRegistry.on_update<Component_t>().template connect<Candidate_t>(instance);
        }

        template <typename Component_t, auto Candidate_t>
        constexpr void disconnectOnUpdate() {
            m_componentsRegistry.on_update<Component_t>().template connect<Candidate_t>();
        }

        template <typename Component_t, auto Candidate_t, typename Instance_t>
        constexpr void disconnectOnUpdate(Instance_t&& instance) {
            m_componentsRegistry.on_update<Component_t>().template disconnect<Candidate_t>(instance);
        }

        // on component removed callback

        template <typename Component_t, auto Candidate_t>
        constexpr void connectOnRemove() {
            m_componentsRegistry.on_destroy<Component_t>().template connect<Candidate_t>();
        }

        template <typename Component_t, auto Candidate_t, typename Instance_t>
        constexpr void connectOnRemove(Instance_t&& instance) {
            m_componentsRegistry.on_destroy<Component_t>().template connect<Candidate_t>(instance);
        }

        template <typename Component_t, auto Candidate_t>
        constexpr void disconnectOnRemove() {
            m_componentsRegistry.on_destroy<Component_t>().template connect<Candidate_t>();
        }

        template <typename Component_t, auto Candidate_t, typename Instance_t>
        constexpr void disconnectOnRemove(Instance_t&& instance) {
            m_componentsRegistry.on_destroy<Component_t>().template disconnect<Candidate_t>(instance);
        }

    private:
        ComponentRegistry_t m_componentsRegistry    {};
        ComponentRegistry_t m_singlecmpsRegistry    {};

        // phony entity to assign the singleton components to
        Entityid_t          m_singlecmpsStorage     {}; 
    };
}
