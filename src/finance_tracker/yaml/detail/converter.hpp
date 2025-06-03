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
        /*
         * there's no other better spot to do validation, so as part of the decoding logic, we will be parsing t
         */

        ::fntr::utils::template_for_each<::fntr::reflect::reflection_type<T>>([&node, &rhs]<typename Descriptor>() {
            using member_type = typename Descriptor::member_type;

            if constexpr (fntr::concepts::container_like<member_type>)
            {
                if (!node[Descriptor::name])
                    return;
                rhs.*(Descriptor::mem_ptr) = node[Descriptor::name].template as<member_type>();
            }
            else if constexpr (fntr::concepts::same_as_optional<member_type>)
            {
                if (!node[Descriptor::name])
                    return;
                rhs.*(Descriptor::mem_ptr) = node[Descriptor::name].template as<typename member_type::value_type>();
            }
            else if constexpr (fntr::concepts::same_as_variant<member_type>)
            {
                // TODO: Support for std::variant
                static_assert(fntr::concepts::always_false<T>, "Not yet implemented");
            }
            else
            {
                rhs.*(Descriptor::mem_ptr) = node[Descriptor::name].template as<member_type>();
            }
        });
        return true;
    }
};

} // namespace YAML
