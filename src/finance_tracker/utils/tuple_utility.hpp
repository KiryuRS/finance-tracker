#pragma once

#include <concepts>
#include <tuple>

namespace fntr::utils {

namespace detail {

template <typename T>
struct is_tuple : std::false_type
{
};

template <typename ... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type
{
};

} // namespace detail

template <typename T>
concept tuple_like = detail::is_tuple<T>::value;

template <tuple_like ... Ts>
using tuple_concat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

template <tuple_like T, typename Predicate>
using tuple_remove_if_t = typename detail::tuple_remove_if<T, Predicate>::type;

}
