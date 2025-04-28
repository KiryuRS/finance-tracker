#pragma once

#include <cstdint>

namespace fntr::messages::canonical {

namespace detail {

inline int16_t make_version(int8_t major, int8_t minor) { return (major << 8) + minor; }

} // namespace detail

enum class message_type : int16_t
{
    EXPENSE_INCOME_ITEM,
};

struct header
{
    uint64_t size{};
    uint16_t version{};
    canonical::message_type message_type{};
};

static_assert(sizeof(header) == 16);

} // namespace fntr::messages::canonical
