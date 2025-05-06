#pragma once

#include <finance_tracker/utils/static_vector.hpp>

#include <iostream>
#include <string_view>

namespace fntr::utils {

template <typename CharT, std::size_t Size>
class basic_static_string : public static_vector<CharT, Size>
{
    using base = static_vector<CharT, Size>;

public:
    using value_type      = typename base::value_type;
    using size_type       = typename base::size_type;
    using reference       = typename base::reference;
    using const_reference = typename base::const_reference;
    using pointer         = typename base::pointer;
    using const_pointer   = typename base::const_pointer;

public:
    explicit constexpr basic_static_string() = default;

    explicit constexpr basic_static_string(std::basic_string_view<CharT> str)
        : base{std::span{str.data(), str.size()}}
    {}

    template <typename T = const CharT*>
        requires(!std::is_array_v<T>) && std::is_pointer_v<T>
    explicit constexpr basic_static_string(T str)
        : basic_static_string{std::string_view{str}}
    {}

    template <std::size_t N>
        requires(N <= Size)
    constexpr basic_static_string(const CharT (&data)[N])
        : base(data)
    {}

public:
    [[nodiscard]] constexpr operator std::string_view() const { return {base::data_, Size}; }

    friend std::ostream& operator<<(std::ostream& os, const basic_static_string& bss)
    {
        return os << static_cast<std::string_view>(bss);
    }

public:
    auto operator<=>(const basic_static_string&) const = default;
};

template <std::size_t Size>
using static_string = basic_static_string<char, Size>;

// CTAD
template <size_t N>
basic_static_string(const char (&data)[N]) -> basic_static_string<char, N>;

} // namespace fntr::utils
