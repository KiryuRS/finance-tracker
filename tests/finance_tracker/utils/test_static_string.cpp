#include <finance_tracker/utils/static_string.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string_view>

namespace fntr::tests {

using namespace ::testing;

TEST(static_string, test_constructors)
{
    using namespace std::literals;
    constexpr utils::static_string<11> s1{"Hello World"sv};
    EXPECT_EQ(s1, "Hello World"sv);

    // extra null terminating character
    constexpr utils::static_string<20> s2{"Goodbye World"};
    EXPECT_EQ(s2, "Goodbye World");

    constexpr char arr[]{'A', 'B', 'C', 'D'};
    constexpr utils::static_string<10> s3{arr};
    EXPECT_THAT(s3, ElementsAreArray(arr));
}

} // namespace fntr::tests
