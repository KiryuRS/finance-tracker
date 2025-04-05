#pragma once

#include <concepts>
#include <chrono>
#include <utility>

namespace fntr::concepts {

namespace detail {

static constexpr uint64_t DAY_IN_SECONDS = 86400;

template <typename T>
struct is_chrono_time_point : std::false_type
{
};

template <typename T, typename Rep, size_t Num, size_t Denom>
struct is_chrono_time_point<std::chrono::time_point<T, std::chrono::duration<Rep, std::ratio<Num, Denom>>>>
{
    static constexpr bool value = Num != DAY_IN_SECONDS && Denom != 1;
};

template <typename T>
struct is_chrono_date : std::false_type
{
};

template <typename T, typename Rep, size_t Num>
struct is_chrono_date<std::chrono::time_point<T, std::chrono::duration<Rep, std::ratio<Num, 1>>>>
{
    static constexpr bool value = Num == DAY_IN_SECONDS;
};

}

template <typename Enum, typename T = std::remove_cvref_t<Enum>>
concept same_as_enum = std::is_enum_v<T>;

template <typename T, typename Other>
concept same_as_cvref_t = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<Other>>;

template <typename T>
concept same_as_time_point = detail::is_chrono_time_point<std::remove_cvref_t<T>>::value;

template <typename T>
concept same_as_date = detail::is_chrono_date<std::remove_cvref_t<T>>::value;

template <typename T>
concept same_as_built_in_type = std::integral<T> || std::floating_point<T>;

}
