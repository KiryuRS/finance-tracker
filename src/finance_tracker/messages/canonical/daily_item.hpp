#pragma once

#include <fntr/messages/canonical/common.hpp>

namespace fntr::messages::canonical {

// determines the categorization when classifying each expense_income_item
enum class category_level : int16_t
{
    NONE      = 0,
    PRIMARY   = 1 << 0,
    SECONDARY = 1 << 1,
};

struct category
{
};

} // namespace fntr::messages::canonical
