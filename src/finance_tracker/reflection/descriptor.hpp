#pragma once

#include <finance_tracker/concepts/traits.hpp>
#include <finance_tracker/utils/type_traits.hpp>
#include <finance_tracker/utils/utility.hpp>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>

#include <format>
#include <iomanip>
#include <sstream>
#include <string_view>
#include <tuple>

namespace fntr::reflect {

namespace detail {

template <typename, auto>
struct descriptor;

} // namespace detail

template <typename>
consteval auto get_tuple_descriptors()
{
    static_assert("Reflection for T is not implemented. Use FNTR_REFLECT on T to enable support");
}

template <typename T>
using reflection_type = decltype(get_tuple_descriptors<T>());

template <typename T>
std::string to_string(const T&);

// operator<< for containers
template <concepts::container_like T>
std::ostream& operator<<(std::ostream& os, [[maybe_unused]] const T& rhs)
{
    std::string delimiter;
        if constexpr (concepts::same_as_vector<T>) {
            os << '[';
            for (const auto& elem : rhs)
                os << std::exchange(delimiter, ", ") << elem;
            os << ']';
        }
        else if constexpr (concepts::same_as_unordered_map<T>) {
            os << '[';
            for (const auto& [key, value] : rhs)
                os << std::exchange(delimiter, ", ") << "{" << key << ":" << value << "}";
            os << ']';
        }
        else if constexpr (concepts::same_as_unordered_set<T>) {
            os << '{';
            for (const auto& elem : rhs)
                os << std::exchange(delimiter, ", ") << elem;
            os << '}';
        }
        else {
            static_assert(concepts::always_false<T>);
        }
    return os;
}

// operator<< for optional
std::ostream& operator<<(std::ostream& os, const concepts::same_as_optional auto& rhs)
{
    if (rhs.has_value()) return os << *rhs;
    return os << "<not-set>";
}

} // namespace fntr::reflect

#define MEMBER_DESCRIPTOR(r, Class, Index, Member)                                                                     \
    template <>                                                                                                        \
    struct descriptor<Class, &Class::Member>                                                                           \
    {                                                                                                                  \
        using tag                 = descriptor;                                                                        \
        using class_type          = Class;                                                                             \
        using member_type         = decltype(Class::Member);                                                           \
        using member_pointer_type = member_type(Class::*);                                                             \
                                                                                                                       \
        static constexpr auto index                  = Index;                                                          \
        static constexpr std::string_view name       = BOOST_PP_STRINGIZE(Member);                                     \
        static constexpr std::string_view type_str   = fntr::utils::full_type_to_str<member_type>();                   \
        static constexpr member_pointer_type mem_ptr = &Class::Member;                                                 \
    };

#define CREATE_DESCRIPTOR(r, Class, Member) detail::descriptor<Class, &Class::Member>{},

#define FNTR_REFLECT(Class, ...)                                                                                       \
    namespace fntr::reflect {                                                                                          \
    namespace detail {                                                                                                 \
    BOOST_PP_SEQ_FOR_EACH_I(MEMBER_DESCRIPTOR, Class, BOOST_PP_TUPLE_TO_SEQ(__VA_ARGS__));                             \
    } /* namespace detail */                                                                                           \
                                                                                                                       \
    template <>                                                                                                        \
    consteval auto get_tuple_descriptors<Class>()                                                                      \
    {                                                                                                                  \
        return std::tuple{BOOST_PP_SEQ_FOR_EACH(CREATE_DESCRIPTOR, Class, BOOST_PP_TUPLE_TO_SEQ(__VA_ARGS__))};        \
    }                                                                                                                  \
                                                                                                                       \
    template <typename Descriptor>                                                                                     \
    const auto& get_member(const Class& object)                                                                        \
    {                                                                                                                  \
        return object.*(Descriptor::mem_ptr);                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    template <typename Descriptor>                                                                                     \
    auto& get_member(Class& object)                                                                                    \
    {                                                                                                                  \
        return object.*(Descriptor::mem_ptr);                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    template <>                                                                                                        \
    std::string to_string<Class>(const Class& object)                                                                  \
    {                                                                                                                  \
        std::ostringstream oss;                                                                                        \
        std::string delimiter;                                                                                         \
        oss << "{ " << BOOST_PP_STRINGIZE(Class) << " {";                                                              \
        constexpr auto descriptors = get_tuple_descriptors<Class>();                                                   \
        fntr::utils::template_for_each(descriptors, [&oss, &object, &delimiter]<typename Descriptor>() {               \
            oss << std::fixed << std::setprecision(3) << std::exchange(delimiter, ", ") << "'" << Descriptor::name     \
                << "': " << get_member<Descriptor>(object);                                                            \
        });                                                                                                            \
        oss << "} }";                                                                                                  \
        return oss.str();                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    std::ostream& operator<<(std::ostream& os, const Class& object)                                                    \
    {                                                                                                                  \
        return os << to_string(object);                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    } /* namespace fntr::reflect */                                                                                    \
                                                                                                                       \
    namespace std {                                                                                                    \
    template <>                                                                                                        \
    struct formatter<Class> : formatter<string>                                                                        \
    {                                                                                                                  \
        auto format(const Class& object, format_context& ctx) const                                                    \
        {                                                                                                              \
            return formatter<string>::format(fntr::reflect::to_string<Class>(object), ctx);                            \
        }                                                                                                              \
    };                                                                                                                 \
    } /* namespace std */
