#pragma once

#include <finance_tracker/yaml/detail/converter.hpp>

namespace fntr::yaml {

template <concepts::config_serializable T>
T serialize_from(const std::string& yaml_config_str, const std::string& key)
{
    const YAML::Node node = YAML::Load(yaml_config_str);
    return node[key].as<T>();
}

} // namespace fntr::yaml
