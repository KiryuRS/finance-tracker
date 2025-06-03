#pragma once

#include <finance_tracker/concepts/traits.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <span>
#include <utility>

namespace fntr::utils {

namespace detail {

template <typename T, size_t Size>
    requires concepts::trivially_copyable<T> && std::default_initializable<T>
class static_vector_base
{
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;

public:
    template <typename U>
    struct iterator_impl
    {
        using value_type        = U;
        using element_type      = U;
        using iterator_category = std::contiguous_iterator_tag;
        using difference_type   = std::ptrdiff_t;

        explicit iterator_impl(value_type* start = nullptr)
            : ptr_{start}
        {}

        value_type& operator*() const { return *ptr_; }

        value_type& operator[](int index) const { return *(ptr_ + index); }

        value_type* operator->() const { return ptr_; }

        iterator_impl& operator++()
        {
            ++ptr_;
            return *this;
        }

        iterator_impl operator++(int)
        {
            auto tmp = ptr_;
            ++ptr_;
            return tmp;
        }

        iterator_impl& operator+=(int i)
        {
            ptr_ += i;
            return *this;
        }

        iterator_impl operator+(int i) const
        {
            auto tmp = ptr_ + i;
            return tmp;
        }

        friend iterator_impl operator+(difference_type value, iterator_impl it) { return it + value; }

        iterator_impl& operator--()
        {
            --ptr_;
            return *this;
        }

        iterator_impl operator--(int)
        {
            auto tmp = *ptr_;
            --ptr_;
            return tmp;
        }

        iterator_impl& operator-=(int i)
        {
            ptr_ -= i;
            return *this;
        }

        iterator_impl operator-(int i) const
        {
            auto tmp = ptr_ - i;
            return tmp;
        }

        friend iterator_impl operator-(difference_type value, iterator_impl it) { return it - value; }

        difference_type operator-(iterator_impl it) const { return ptr_ - it.ptr_; }

        auto operator<=>(const iterator_impl&) const = default;

    private:
        value_type* ptr_;
    };

    using iterator       = iterator_impl<value_type>;
    using const_iterator = iterator_impl<const value_type>;

public:
    constexpr static_vector_base()
        : data_{}
        , length_{0}
    {}

    template <std::ranges::range U>
    explicit constexpr static_vector_base(const U& container)
        : data_{}
        , length_{std::ranges::size(container)}
    {
        if (length_ > Size) throw std::out_of_range("container size exceeds capacity of static_vector");
        initialize_data(container);
    }

    template <std::size_t N>
        requires(N <= Size)
    explicit constexpr static_vector_base(const T (&data)[N])
        : data_{}
        , length_{N}
    {
        initialize_data(std::span{data, N});
    }

    constexpr static_vector_base(std::initializer_list<T> list)
        : data_{}
        , length_{list.size()}
    {
        if (length_ > Size) throw std::out_of_range("container size exceeds capacity of static_vector");
        initialize_data(list);
    }

    constexpr static_vector_base(const static_vector_base& other)
        : data_{}
        , length_{other.length_}
    {
        initialize_data(std::span{other.data_, length_});
    }

    constexpr static_vector_base(static_vector_base&& other) noexcept
        : data_{std::exchange(other.data_, nullptr)}
        , length_{std::exchange(other.length_, 0)}
    {}

    auto operator<=>(const static_vector_base& other) const = default;

public:
    [[nodiscard]] constexpr size_type size() const { return length_; }

    [[nodiscard]] constexpr bool full() const { return length_ >= Size; }

    [[nodiscard]] constexpr bool empty() const { return length_ == 0; }

public:
    iterator begin() { return iterator{data_}; }

    const_iterator begin() const { return const_iterator{data_}; }

    const_iterator cbegin() const { return const_iterator{data_}; }

    iterator end() { return iterator{data_ + length_}; }

    const_iterator end() const { return const_iterator{data_ + length_}; }

    const_iterator cend() const { return const_iterator{data_ + length_}; }

private:
    constexpr void initialize_data(const std::ranges::range auto& out_data)
    {
        // TODO: Use constexpr placement new in C++26
        // for now, okay to use std::construct_at as long as it supports move/copy construction
        std::ranges::for_each(out_data, [this, i = 0](const auto& value) mutable { data_[i++] = value; });
    }

protected:
    static_assert(std::contiguous_iterator<iterator>);
    static_assert(std::contiguous_iterator<const_iterator>);

    T data_[Size];
    std::size_t length_;
};

} // namespace detail

// behaves like array, but with vector's API (ignore front APIs)
template <typename T, size_t Size>
class static_vector : public detail::static_vector_base<T, Size>
{
    using base = detail::static_vector_base<T, Size>;

public:
    using value_type      = typename base::value_type;
    using size_type       = typename base::size_type;
    using reference       = typename base::reference;
    using const_reference = typename base::const_reference;
    using pointer         = typename base::pointer;
    using const_pointer   = typename base::const_pointer;

    using base::base;

    auto operator<=>(const static_vector&) const = default;

    // WARN: undefined behavior if empty() == true
    // TODO: Use contracts in C++26?
    void pop_back() { (base::data_ + --base::length_)->~T(); }

    // WARN: undefined behavior if length_ >= Size
    // TODO: Use contracts in C++26?
    template <std::convertible_to<T> U>
    void push_back(U&& value)
    {
        emplace_back(std::forward<U>(value));
    }

    // WARN: undefined behavior if length_ >= Size
    // TODO: Use contracts in C++26?
    template <typename... Args>
        requires std::constructible_from<T, Args...>
    reference emplace_back(Args&&... args)
    {
        return *new (base::data_ + base::length_++) T{std::forward<Args>(args)...};
    }

    template <typename Self>
    [[nodiscard]] decltype(auto) operator[](this Self&& self, size_type index)
    {
        return std::forward<Self>(self).data_[index];
    }

    template <typename Self>
    [[nodiscard]] decltype(auto) at(this Self&& self, size_type index)
    {
        if (index >= self.length_)
            throw std::out_of_range("index beyond length");
        return std::forward<Self>(self).data_[index];
    }
};

} // namespace fntr::utils
