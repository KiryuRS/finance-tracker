#pragma once

#include <finance_tracker/concepts/utility.hpp>

namespace fntr::utils {

namespace detail {

template <typename Functor, typename T>
concept type_invocable = requires { std::declval<Functor>().template operator()<T>(); };

template <typename... Ts>
constexpr void template_for_each_impl(auto&& lambda)
{
    constexpr auto type_visitor = []<typename T>(auto&& lambda) {
        using ftype = std::remove_cvref_t<decltype(lambda)>;

        if constexpr (detail::type_invocable<ftype, T>)
            std::forward<decltype(lambda)>(lambda).template operator()<T>();
        else if constexpr (std::invocable<ftype>)
            std::forward<decltype(lambda)>(lambda)();
        else
            static_assert(concepts::always_false<T>, "Lambda is not invocable!");
    };
    (type_visitor.template operator()<Ts>(std::forward<decltype(lambda)>(lambda)), ...);
}

template <typename F, template <typename...> typename L, typename... Ts>
constexpr void template_generic_for_each_impl(L<Ts...>, F&& functor)
{
    template_for_each_impl<Ts...>(std::forward<F>(functor));
}

} // namespace detail

// one or more types
template <typename... Ts>
constexpr void template_types_for_each(auto&& lambda)
{
    detail::template_for_each_impl<Ts...>(std::forward<decltype(lambda)>(lambda));
}

// either std::tuple, or typelist similar
template <typename T>
constexpr void template_for_each(auto&& lambda)
{
    detail::template_generic_for_each_impl(T{}, std::forward<decltype(lambda)>(lambda));
}

template <typename T>
constexpr void template_for_each(const T&, auto&& lambda)
{
    return template_for_each<T>(std::forward<decltype(lambda)>(lambda));
}

} // namespace fntr::utils
