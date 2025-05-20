#pragma once

#include <concepts>
#include <utility>

namespace fntr::concepts {

template <typename T, typename RawT = std::remove_cvref_t<T>>
concept config_serializable = std::default_initializable<RawT>;

}
