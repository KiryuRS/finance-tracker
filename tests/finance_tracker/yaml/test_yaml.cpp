#include <finance_tracker/yaml/serializer.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace fntr::tests::mocks {

struct built_in_types
{
    float floating_value;
    int integer_value;
    char char_value;
    double double_value;
    long long_value;

    auto operator<=>(const built_in_types&) const = default;
};

struct simple
{
    int value_1;

    auto operator<=>(const simple&) const = default;
};

struct complex_types
{
    std::vector<int> vector;
    std::string string;
    std::unordered_map<int, int> unordered_map;
    std::optional<double> optional;
    std::vector<double> empty_vector;
    std::optional<int> empty_optional;
    simple s;

    auto operator<=>(const complex_types&) const = default;
};

} // namespace fntr::tests::mocks

FNTR_REFLECT(fntr::tests::mocks::built_in_types, (floating_value, integer_value, char_value, double_value, long_value));
FNTR_REFLECT(fntr::tests::mocks::simple, (value_1));
FNTR_REFLECT(
    fntr::tests::mocks::complex_types, (vector, string, unordered_map, optional, empty_vector, empty_optional, s)
);

namespace fntr::tests {

using namespace ::testing;

TEST(test_yaml_conversion, test_serializable_built_in_type_wrapped)
{
    const std::string& yaml_str = R"(
built_in:
    floating_value: 111.2
    integer_value: 20
    char_value: 'A'
    double_value: 3.14
    long_value: 99712
)";

    const auto serialized = yaml::serialize_from<mocks::built_in_types>(yaml_str, "built_in");
    const mocks::built_in_types expected{
        .floating_value = 111.2f, .integer_value = 20, .char_value = 'A', .double_value = 3.14, .long_value = 99712};
    EXPECT_EQ(serialized, expected);
}

TEST(test_yaml_conversion, test_serializable_complex_type)
{
    const std::string& yaml_str = R"(
complex:
    vector: [1,2,3,4,5]
    s:
        value_1: 10
    unordered_map:
        1: 10
        2: 20
    string: "Hello World"
    optional: 69.69
)";

    const auto serialized = yaml::serialize_from<mocks::complex_types>(yaml_str, "complex");
    EXPECT_THAT(serialized.vector, ElementsAre(1, 2, 3, 4, 5));
    EXPECT_THAT(serialized.unordered_map, UnorderedElementsAre(Pair(1, 10), Pair(2, 20)));
    EXPECT_EQ(serialized.optional, std::optional<double>(69.69));
    EXPECT_EQ(serialized.string, "Hello World");
    EXPECT_EQ(serialized.s.value_1, 10);

    EXPECT_TRUE(serialized.empty_vector.empty());
    EXPECT_FALSE(serialized.empty_optional.has_value());
}

} // namespace fntr::tests
