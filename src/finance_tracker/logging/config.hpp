#pragma once

#include <finance_tracker/reflection/descriptor.hpp>

#include <spdlog/common.h>

#include <filesystem>
#include <optional>
#include <unordered_set>

namespace fntr::logging {

enum class log_level : int
{
    trace    = spdlog::level::level_enum::trace,
    debug    = spdlog::level::level_enum::debug,
    info     = spdlog::level::level_enum::info,
    warn     = spdlog::level::level_enum::warn,
    error    = spdlog::level::level_enum::err,
    critical = spdlog::level::level_enum::critical,
    off      = spdlog::level::level_enum::off,
};

struct file_config
{
    std::filesystem::path filepath;
    std::optional<std::string> symlink;
};

struct logging_filters
{
    std::unordered_set<std::string> modules;
};

struct backtrace_config
{
    bool enabled{false};
    uint64_t messages_count{32};
};

struct config
{
    std::optional<file_config> daily_file;
    log_level level{log_level::info};
    bool stdout{true};
    std::optional<logging_filters> filters;
    std::optional<backtrace_config> backtrace;
};

} // namespace fntr::logging

FNTR_GENERATE_DESCRIPTORS(fntr::logging::config, (daily_file, level, stdout, filters, backtrace));
