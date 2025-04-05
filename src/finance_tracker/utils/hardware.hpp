#pragma once

#include <climits>
#include <cstring>
#include <string>

extern "C" {
#include <unistd.h>
}

namespace fntr::utils::hardware {

namespace detail {

inline std::string get_hostname()
{
    char buffer[HOST_NAME_MAX]{};
    ::gethostname(buffer, HOST_NAME_MAX);
    return std::string{buffer, std::strlen(buffer)};
}

}

inline const std::string application_hostname = detail::get_hostname();

}
