#include <finance_tracker/utils/static_vector.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <ranges>
#include <string_view>

namespace fntr::tests {

using namespace ::testing;

TEST(static_vector, test_constructor_built_in_types)
{
    using static_vector_t = utils::static_vector<int, 10>;
    static_assert(std::ranges::range<static_vector_t>);

    const static_vector_t v;
    EXPECT_TRUE(v.empty());

    const std::array<int, 10> arr{1, 2, 3, 4, 5};
    const static_vector_t v1{arr};
    EXPECT_THAT(v1, ElementsAreArray(arr));
    EXPECT_EQ(v1.size(), arr.size());
    EXPECT_FALSE(v1.empty());
    EXPECT_TRUE(v1.full());

    utils::static_vector<char, 7> v2{{'a', 'b'}};
    EXPECT_THAT(v2, ElementsAre('a', 'b'));
    EXPECT_EQ(v2.size(), 2);
    EXPECT_FALSE(v2.empty());
    EXPECT_FALSE(v2.full());
}

TEST(static_vector, test_constexprness)
{
    constexpr double arr[100]{1.1, 1.2, 1.3, {}};
    constexpr utils::static_vector<double, 100> v1{arr};
    EXPECT_EQ(v1.size(), 100);
    EXPECT_THAT(v1, ElementsAreArray(arr));

    constexpr utils::static_vector<char, 2> v2;
    EXPECT_TRUE(v2.empty());

    using namespace std::literals;
    constexpr utils::static_vector<std::string_view, 5> v3{{"Hello World", "Goodbye World", "This is Sparta"}};
    EXPECT_EQ(v3.size(), 3);
    EXPECT_THAT(v3, ElementsAre("Hello World"sv, "Goodbye World"sv, "This is Sparta"sv));
}

TEST(static_vector, test_member_operations)
{
    utils::static_vector<int, 10> v;
    EXPECT_TRUE(v.empty());

    // push / emplace two elements
    v.emplace_back(1);
    v.push_back(2);
    EXPECT_THAT(v, ElementsAre(1, 2));
    EXPECT_EQ(v.size(), 2);
    EXPECT_FALSE(v.empty());
    EXPECT_FALSE(v.full());

    // pop all elements
    v.pop_back();
    v.pop_back();
    EXPECT_TRUE(v.empty());

    // now we add 10 elements to make the container full
    for (int i : std::views::iota(0, 10))
        v.push_back(i);
    EXPECT_THAT(v, ElementsAre(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
    EXPECT_EQ(v.size(), 10);
    EXPECT_FALSE(v.empty());
    EXPECT_TRUE(v.full());

    // access operators
    EXPECT_EQ(v[9], 9);
    EXPECT_EQ(v.at(7), 7);
    EXPECT_THROW(std::ignore = v.at(999), std::out_of_range);
}

TEST(static_vector, test_comparators)
{
    constexpr utils::static_vector<std::string_view, 5> v1{{"Hello World", "Goodbye World", "This is Sparta"}};
    const auto v2{v1};
    EXPECT_EQ(v2, v1);

    const auto v3{std::move(v2)};

    EXPECT_EQ(v3, v1);

    auto v4{v1};
    v4.pop_back();
    EXPECT_NE(v4, v1);
}

} // namespace fntr::tests
