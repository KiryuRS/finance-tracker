#pragma once

#include <finance_tracker/yaml/detail/converter.hpp>
#include <finance_tracker/utils/type_traits.hpp>

#include <iostream>

namespace fntr::yaml {

template <concepts::config_serializable T>
T serialize_from(const std::string& yaml_config_str)
{
    constexpr std::string_view type_name = utils::short_type_to_str<T>();
    const YAML::Node node = YAML::Load(yaml_config_str);
    return node[type_name].as<T>();
}

} // namespace fntr::yaml
