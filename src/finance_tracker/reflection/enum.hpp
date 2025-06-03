#pragma once

#include <finance_tracker/concepts/traits.hpp>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/variadic.hpp>

#include <array>
#include <format>
#include <iostream>
#include <ranges>
#include <string_view>

#define ENUM_STR(r, _, x)                          BOOST_PP_STRINGIZE(x),
#define ENUM_IDENTITY_NAMESPACE(r, e_namespace, x) e_namespace::x,
#define ENUM_IDENTITY(r, _, x)                     x,

#define NUM_ARGS(...) std::array{BOOST_PP_SEQ_FOR_EACH(ENUM_STR, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))}.size()

namespace fntr::reflect {

template <fntr::concepts::same_as_enum Enum>
constexpr Enum from_string(std::string_view);

template <fntr::concepts::same_as_enum Enum>
struct enum_metadata;

} // namespace fntr::reflect

#define ENUM_REFLECTION(enum_namespace, enum_name, ...)                                                                \
    namespace fntr::reflect {                                                                                          \
    template <>                                                                                                        \
    struct enum_metadata<enum_namespace::enum_name>                                                                    \
    {                                                                                                                  \
        static inline constexpr size_t size                                            = NUM_ARGS(__VA_ARGS__);        \
        static inline constexpr std::array<enum_namespace::enum_name, size> enum_array = {BOOST_PP_SEQ_FOR_EACH(       \
            ENUM_IDENTITY_NAMESPACE, enum_namespace::enum_name, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)                  \
        )};                                                                                                            \
        static inline constexpr std::array<std::string_view, size> enum_str_array      = {                             \
            BOOST_PP_SEQ_FOR_EACH(ENUM_STR, enum_namespace::enum_name, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))};        \
    };                                                                                                                 \
    inline constexpr std::string_view to_string(enum_namespace::enum_name x)                                           \
    {                                                                                                                  \
        constexpr auto& enum_array     = enum_metadata<enum_namespace::enum_name>::enum_array;                         \
        constexpr auto& enum_str_array = enum_metadata<enum_namespace::enum_name>::enum_str_array;                     \
        constexpr auto size            = enum_metadata<enum_namespace::enum_name>::size;                               \
        const auto iter                = std::ranges::find(enum_array, x);                                             \
        return iter != enum_array.end() ? enum_str_array[iter - enum_array.begin()] : enum_str_array[size - 1];        \
    }                                                                                                                  \
    template <>                                                                                                        \
    enum_namespace::enum_name from_string<enum_namespace::enum_name>(std::string_view x)                               \
    {                                                                                                                  \
        constexpr auto& enum_array     = enum_metadata<enum_namespace::enum_name>::enum_array;                         \
        constexpr auto& enum_str_array = enum_metadata<enum_namespace::enum_name>::enum_str_array;                     \
        constexpr auto size            = enum_metadata<enum_namespace::enum_name>::size;                               \
        const auto iter                = std::ranges::find(enum_str_array, x);                                         \
        return iter != enum_str_array.end() ? enum_array[iter - enum_str_array.begin()] : enum_array[size - 1];        \
    }                                                                                                                  \
    std::ostream& operator<<(std::ostream& os, enum_namespace::enum_name x)                                            \
    {                                                                                                                  \
        return os << to_string(x);                                                                                     \
    }                                                                                                                  \
    } /* namespace fntr::reflect */                                                                                    \
    namespace std {                                                                                                    \
    template <>                                                                                                        \
    struct formatter<enum_namespace::enum_name> : formatter<string>                                                    \
    {                                                                                                                  \
        auto format(enum_namespace::enum_name x, format_context& ctx) const                                            \
        {                                                                                                              \
            return formatter<string>::format(string{fntr::reflect::to_string(x)}, ctx);                                \
        }                                                                                                              \
    };                                                                                                                 \
    } /* namespace std */

/*
 * This should always be used to define an ordinary enum class
 * Usage: DEFINE_ENUM_CLASS(mynamespace::scope, foo, ONE, TWO, THREE)
 *
 * Note that there will always be an implicit NONE introduced with index == last,
 *  i.e. static_cast<int>(mynamespace::scope::foo::NONE) == raf::reflect::enum_metadata::size
 * and this macro provides the following for free:
 *  - fntr::reflect::to_string,
 *  - fntr::reflect::from_string<enum>,
 *  - operator<< overload with ostream,
 *  - formatter
 *  - enum_metadata struct that contains:
 *      # size of the enum (i.e. enum_metadata<foo>::size)
 *      # enum as array (i.e. enum_metadata<foo>::enum_array)
 *      # enum string(_view) as array (i.e. enum_metadata<foo>::enum_str_array)
 */
#define DEFINE_ENUM_CLASS(enum_namespace, enum_name, ...)                                                              \
    namespace enum_namespace {                                                                                         \
    enum class enum_name : uint8_t                                                                                     \
    {                                                                                                                  \
        BOOST_PP_SEQ_FOR_EACH(ENUM_IDENTITY, enum_name, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__, NONE))                   \
    };                                                                                                                 \
    } /* namespace enum_namespace */                                                                                   \
    ENUM_REFLECTION(enum_namespace, enum_name, __VA_ARGS__, NONE)
