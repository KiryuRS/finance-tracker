#pragma once

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

struct config
{
    std::optional<file_config> daily_file;
    log_level level{log_level::info};
    bool stdout{true};
    std::optional<logging_filters> filters;
};

} // namespace fntr::logging
