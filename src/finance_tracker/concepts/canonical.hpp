#pragma once

#include <concepts>
#include <cstdint>
#include <utility>

namespace fntr::concepts {

template <typename T, typename RawT = std::remove_cvref_t<T>>
concept header_compilant = requires(RawT x)
{
    {
        x.version
        } -> std::same_as<uint16_t>;
    {
        x.size
        } -> std::same_as<uint64_t>;
    x.message_type;
};

template <typename T, typename RawT = std::remove_cvref_t<T>>
concept canonical_type = requires(RawT x)
{
    requires header_compilant<decltype(std::declval<RawT::header>())>;
    requires std::is_aggregate_v<RawT>;
    requires std::default_initializable<RawT>;
    requires std::copy_constructible<RawT>;
};

} // namespace fntr::concepts
