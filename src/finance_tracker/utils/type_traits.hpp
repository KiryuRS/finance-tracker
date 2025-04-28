#pragma once

#include <source_location>
#include <string_view>

namespace fntr::utils {

namespace detail {

template <typename T>
consteval const char* get_function_name()
{
    return std::source_location::current().function_name();
}

} // namespace detail

template <typename T>
consteval std::string_view full_type_to_str()
{
    constexpr std::string_view void_source = detail::get_function_name<void>();
    constexpr size_t start_index           = void_source.find("void");
    static_assert(start_index != std::string_view::npos, "Please revisit me!");

    constexpr std::string_view type_source = detail::get_function_name<T>();
    constexpr size_t end_index             = type_source.find("]", start_index);
    static_assert(
        end_index != std::string_view::npos, "Please revisit me! GCC/clang changed their underlying implementation"
    );

    constexpr auto type = type_source.substr(start_index, end_index - start_index);
    return type;
}

// this removes the namespaces
// e.g. std::vector<int> -> vector<int>
//      some::long::namespace::vector<std::string> -> vector<std::string>
//      foo<some_class::deep::within>::type -> type
template <typename T>
consteval std::string_view short_type_to_str()
{
    constexpr std::string_view full_type = full_type_to_str<T>();
    size_t levels = 0, pos = std::string_view::npos;
        for (size_t i = 0; i != full_type.length(); ++i) {
            char c = full_type[i];
            if (c == '<')
                ++levels;
            else if (c == '>')
                --levels;

            if (levels == 0 && c == ':') pos = i;
        }
    return pos == std::string_view::npos ? full_type : full_type.substr(pos + 1);
}

// this removes the last template argument on the "outer" level
// e.g. std::vector<int> -> std::vector
//      some::long::namespace::vector<std::string> -> some::long::namespace::vector
//      foo<some_class::deep::within>::type -> foo
//      foo<another<boo>> -> foo
template <typename T>
consteval std::string_view templateless_type_to_str()
{
    constexpr std::string_view full_type = full_type_to_str<T>();
    size_t levels = 0, pos = std::string_view::npos;
        for (size_t i = 0; i != full_type.length(); ++i) {
            char c = full_type[i];
            if (c == '<')
                ++levels;
            else if (c == '>')
                --levels;

            if (levels == 1 && c == '<') pos = i;
        }
    return pos == std::string_view::npos ? full_type : full_type.substr(0, pos);
}

} // namespace fntr::utils
