#pragma once

#include <spdlog/common.h>

#include <filesystem>
#include <optional>

namespace fntr::logging {

enum class log_level : int
{
    trace = spdlog::level::level_enum::trace,
    debug = spdlog::level::level_enum::debug,
    info = spdlog::level::level_enum::info,
    warn = spdlog::level::level_enum::warn,
    error = spdlog::level::level_enum::err,
    critical = spdlog::level::level_enum::critical,
    off = spdlog::level::level_enum::off,
};

struct file_config
{
    std::filesystem::path filepath;
    std::optional<std::string> symlink;
};

struct config
{
    std::optional<file_config> daily_file;
    log_level level;
    bool stdout;
};

}
