#pragma once

#include <finance_tracker/concepts/config.hpp>
#include <finance_tracker/concepts/traits.hpp>
#include <finance_tracker/reflection/descriptor.hpp>
#include <finance_tracker/utils/utility.hpp>

#include <yaml-cpp/yaml.h>

namespace YAML {

template <fntr::concepts::config_serializable T>
struct convert<T>
{
    static Node encode([[maybe_unused]] const T& rhs)
    {
        Node node{};
        // TODO: Implement me
        return node;
    }

    static bool decode(const Node& node, T& rhs)
    {
        // TODO: containers should be treated as optionals (i.e. if does not exist, skip)
        ::fntr::utils::template_for_each<::fntr::reflect::reflection_type<T>>([&node, &rhs] <typename Descriptor> () {
            using member_type = typename Descriptor::member_type;
            rhs.*(Descriptor::mem_ptr) = node[Descriptor::name].template as<member_type>();
        });
        return true;
    }
};

}
