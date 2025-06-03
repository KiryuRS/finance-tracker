#include <finance_tracker/utils/type_traits.hpp>

#include <gtest/gtest.h>

#include <array>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace fntr::tests {

namespace short_namespace {

struct dummy
{
};

} // namespace short_namespace

namespace super::duper::in_long::in_namespace {

struct some_dummy_type
{
};

} // namespace super::duper::in_long::in_namespace

template <typename...>
struct typelist;

using namespace ::testing;

TEST(test_type_to_str, test_full_suite)
{
    using namespace std::literals;
    static_assert(utils::full_type_to_str<std::vector<int>>() == "std::vector<int>"sv);
    static_assert(utils::full_type_to_str<double>() == "double"sv);
    static_assert(
        utils::full_type_to_str<super::duper::in_long::in_namespace::some_dummy_type>() ==
        "fntr::tests::super::duper::in_long::in_namespace::some_dummy_type"sv
    );

    static_assert(utils::short_type_to_str<std::vector<int>>() == "vector<int>"sv);
    static_assert(
        utils::short_type_to_str<std::array<short_namespace::dummy, 5>>() ==
        "array<fntr::tests::short_namespace::dummy, 5>"sv
    );
    static_assert(utils::short_type_to_str<int>() == "int"sv);

    static_assert(utils::templateless_type_to_str<int>() == "int"sv);
    static_assert(utils::templateless_type_to_str<std::vector<int>>() == "std::vector"sv);
    static_assert(utils::templateless_type_to_str<typelist<int, char, double>>() == "fntr::tests::typelist"sv);
    static_assert(
        utils::templateless_type_to_str<typelist<std::vector<int>, std::array<std::string, 5>>>() ==
        "fntr::tests::typelist"sv
    );
}

} // namespace fntr::tests
