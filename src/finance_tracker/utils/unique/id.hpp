#pragma once

#include <chrono>
#include <cstdint>

namespace fntr::utils::unique {

namespace detail {

template <typename, auto>
struct counter
{
    using tag = counter;

    struct generator
    {
        friend consteval auto is_defined(tag) { return true; }
    };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-template-friend"
    friend consteval auto is_defined(tag);
#pragma GCC diagnostic pop

    // friend injection and perform instantiation only when necessary
    template <typename Tag = tag, auto = is_defined(Tag{})>
    static consteval auto exists(auto)
    {
        return true;
    }

    static consteval auto exists(...) { return generator{}, false; };
};

} // namespace detail

// lambda default argument is necessary to force compiler to re-evaulate the conditional statement for each templated
// call without it, the compiler would cache the result and defeats the purpose of a compile time id generator

// this version accepts with a type - useful for reflection and generating unique id for each member
template <typename T, auto Id = uint64_t{}, typename = decltype([] {})>
consteval auto typed_unique_id()
{
    if constexpr (!detail::counter<T, Id>::exists(Id))
        return Id;
    else
        return typed_unique_id<T, Id + 1>();
}

// this version is generic unique id generator
template <auto Id = uint64_t{}, typename = decltype([] {})>
consteval auto unique_id()
{
    if constexpr (!detail::counter<char, Id>::exists(Id))
        return Id;
    else
        return unique_id<char, Id + 1>();
}

} // namespace fntr::utils::unique