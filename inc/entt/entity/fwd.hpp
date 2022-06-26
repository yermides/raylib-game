#ifndef ENTT_ENTITY_FWD_HPP
#define ENTT_ENTITY_FWD_HPP

#include <memory>
#include "../core/fwd.hpp"
#include "../core/type_traits.hpp"

namespace entt {

/*! @brief Default entity identifier. */
enum class entity : id_type {};

template<typename Entity = entity, typename = std::allocator<Entity>>
class basic_sparse_set;

template<typename Type, typename = entity, typename = std::allocator<Type>, typename = void>
class basic_storage;

template<typename, typename = entity, typename = void>
struct storage_type;

template<typename, typename = entity>
struct storage_for;

template<typename Type>
class sigh_storage_mixin;

template<typename = entity>
class basic_registry;

template<typename, typename, typename, typename = void>
class basic_view;

template<typename>
struct basic_runtime_view;

template<typename, typename, typename, typename>
class basic_group;

template<typename>
class basic_observer;

template<typename>
class basic_organizer;

template<typename, typename...>
struct basic_handle;

template<typename>
class basic_snapshot;

template<typename>
class basic_snapshot_loader;

template<typename>
class basic_continuous_loader;

/**
 * @brief Alias for exclusion lists.
 * @tparam Type List of types.
 */
template<typename... Type>
using exclude_t = type_list<Type...>;

/**
 * @brief Variable template for exclusion lists.
 * @tparam Type List of types.
 */
template<typename... Type>
inline constexpr exclude_t<Type...> exclude{};

/**
 * @brief Alias for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
using get_t = type_list<Type...>;

/**
 * @brief Variable template for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
inline constexpr get_t<Type...> get{};

/**
 * @brief Alias for lists of owned components.
 * @tparam Type List of types.
 */
template<typename... Type>
using owned_t = type_list<Type...>;

/**
 * @brief Variable template for lists of owned components.
 * @tparam Type List of types.
 */
template<typename... Type>
inline constexpr owned_t<Type...> owned{};

/*! @brief Alias declaration for the most common use case. */
using sparse_set = basic_sparse_set<>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Type Type of objects assigned to the entities.
 */
template<typename Type>
using storage = basic_storage<Type>;

/*! @brief Alias declaration for the most common use case. */
using registry = basic_registry<>;

/*! @brief Alias declaration for the most common use case. */
using observer = basic_observer<registry>;

/*! @brief Alias declaration for the most common use case. */
using organizer = basic_organizer<registry>;

/*! @brief Alias declaration for the most common use case. */
using handle = basic_handle<registry>;

/*! @brief Alias declaration for the most common use case. */
using const_handle = basic_handle<const registry>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Args Other template parameters.
 */
template<typename... Args>
using handle_view = basic_handle<registry, Args...>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Args Other template parameters.
 */
template<typename... Args>
using const_handle_view = basic_handle<const registry, Args...>;

/*! @brief Alias declaration for the most common use case. */
using snapshot = basic_snapshot<registry>;

/*! @brief Alias declaration for the most common use case. */
using snapshot_loader = basic_snapshot_loader<registry>;

/*! @brief Alias declaration for the most common use case. */
using continuous_loader = basic_continuous_loader<registry>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Get Types of components iterated by the view.
 * @tparam Exclude Types of components used to filter the view.
 */
template<typename Get, typename Exclude = exclude_t<>>
using view = basic_view<entity, Get, Exclude>;

/*! @brief Alias declaration for the most common use case. */
using runtime_view = basic_runtime_view<sparse_set>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Args Other template parameters.
 */
template<typename... Args>
using group = basic_group<entity, Args...>;

} // namespace entt

#endif
