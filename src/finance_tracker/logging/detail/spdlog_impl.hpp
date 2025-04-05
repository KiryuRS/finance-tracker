#pragma once

#include <finance_tracker/utils/hardware.hpp>

#include <spdlog/pattern_formatter.h>

namespace fntr::logging::detail {

namespace formatter {

class hostname_flag : public spdlog::custom_flag_formatter
{
public:
    void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override
    {
        const auto& hostname = utils::hardware::application_hostname;
        dest.append(hostname.data(), hostname.data() + hostname.size());
    }

    std::unique_ptr<spdlog::custom_flag_formatter> clone() const override
    {
        return spdlog::details::make_unique<hostname_flag>();
    }
};

}

}
