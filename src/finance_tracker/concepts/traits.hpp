#pragma once

#include <chrono>
#include <concepts>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

template <typename T>
struct is_vector : std::false_type
{
};

template <typename... Args>
struct is_vector<std::vector<Args...>> : std::true_type
{
};

template <typename T>
struct is_unordered_map : std::false_type
{
};

template <typename... Args>
struct is_unordered_map<std::unordered_map<Args...>> : std::true_type
{
};

template <typename T>
struct is_unordered_set : std::false_type
{
};

template <typename... Args>
struct is_unordered_set<std::unordered_set<Args...>> : std::true_type
{
};

template <typename T>
struct is_optional : std::false_type
{
};

template <typename... Args>
struct is_optional<std::optional<Args...>> : std::true_type
{
};

template <typename T>
struct is_variant_like : std::false_type
{
};

template <typename... Args>
struct is_variant_like<std::variant<Args...>> : std::true_type
{
};

} // namespace detail

template <typename Container, typename T = std::remove_cvref_t<Container>>
concept container_like = !
std::same_as<std::string, T> && !std::same_as<std::string_view, T> && requires(T x) {
                                                                          typename T::value_type;
                                                                          typename T::allocator_type;
                                                                          requires std::ranges::range<T>;
                                                                      };

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

template <typename T>
concept same_as_class_type = std::is_class_v<std::remove_cvref_t<T>>;

template <typename T>
concept same_as_vector = detail::is_vector<std::remove_cvref_t<T>>::value;

template <typename T>
concept same_as_unordered_map = detail::is_unordered_map<std::remove_cvref_t<T>>::value;

template <typename T>
concept same_as_unordered_set = detail::is_unordered_set<std::remove_cvref_t<T>>::value;

template <typename T>
concept same_as_optional = detail::is_optional<std::remove_cvref_t<T>>::value;

template <typename T>
concept same_as_variant = detail::is_variant_like<std::remove_cvref_t<T>>::value;

template <typename T>
concept trivially_moveable = std::is_trivially_move_assignable_v<T>;

template <typename T>
concept trivially_copyable = std::is_trivially_copy_assignable_v<T>;

template <typename T>
concept trivially_assignable = std::is_trivially_copy_assignable_v<T>;

} // namespace fntr::concepts
