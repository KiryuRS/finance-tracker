#include <finance_tracker/reflection/descriptor.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <string_view>

namespace fntr::tests::mocks {

struct boo
{
    int id;
    double multiplier;

    auto operator<=>(const boo&) const = default;
};

struct foo
{
    boo some_boo;
    std::string name;
    std::string_view hostname;

    auto operator<=>(const foo&) const = default;
};

} // namespace fntr::tests::mocks

FNTR_REFLECT(fntr::tests::mocks::boo, (id, multiplier));
FNTR_REFLECT(fntr::tests::mocks::foo, (some_boo, name, hostname));

namespace fntr::tests {

TEST(test_reflection, test_for_each_descriptor)
{
    mocks::boo in;
    in.id         = 100;
    in.multiplier = 123.4;

    mocks::boo out;
    fntr::utils::template_for_each<fntr::reflect::reflection_type<mocks::boo>>([&out, &in]<typename Descriptor>() {
        out.*Descriptor::mem_ptr = fntr::reflect::get_member<Descriptor>(in);
    });

    EXPECT_EQ(in, out);

    mocks::foo in2;
    in2.some_boo = in;
    in2.name     = "Hello WOrld";
    in2.hostname = "google.com";

    mocks::foo out2;
    fntr::utils::template_for_each<fntr::reflect::reflection_type<mocks::foo>>([&out2, &in2]<typename Descriptor>() {
        out2.*Descriptor::mem_ptr = fntr::reflect::get_member<Descriptor>(in2);
    });

    EXPECT_EQ(in2, out2);
}

} // namespace fntr::tests
