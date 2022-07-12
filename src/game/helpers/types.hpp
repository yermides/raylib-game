#pragma once
// FIXME: maybe this should be a precompiled header, I dunno

#include <vector>
template<typename T>
using Vec_t = std::vector<T>;

#include <unordered_map>
template<typename T1, typename T2>
using Hashmap_t = std::unordered_map<T1, T2>;

#include <list>
template<typename T>
using List_t = std::list<T>;

#include <memory>
template<typename T>
using Scoped_t = std::unique_ptr<T>;
template<typename T>
using Referenced_t = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Scoped_t<T> CreateScoped(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr Referenced_t<T> CreateReferenced(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
