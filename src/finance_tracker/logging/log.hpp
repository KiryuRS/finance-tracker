#pragma once

#include <finance_tracker/logging/config.hpp>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include <boost/container/flat_map.hpp>

namespace fntr::logging {

class spdlog_manager
{
public:
    struct spdlog_module
    {
        std::string_view name;
        spdlog::logger& logger;
    };

public:
    explicit spdlog_manager(const logging::config& conf);

    spdlog_manager(const spdlog_manager&)            = delete;
    spdlog_manager(spdlog_manager&&)                 = delete;
    spdlog_manager& operator=(const spdlog_manager&) = delete;
    spdlog_manager& operator=(spdlog_manager&&)      = delete;

public:
    static inline std::unique_ptr<spdlog_manager> global_logger;

    static void log_message(spdlog::logger& logger, log_level level, std::string message);
    static bool should_log(log_level level);
    static bool should_log(std::string_view logger_name, log_level level);

    static spdlog_module create_logger(std::string_view logger_name);
    static spdlog_module get_logger(std::string_view logger_name);

private:
    static inline boost::container::flat_map<std::string_view, std::shared_ptr<spdlog::logger>> loggers_;

    const logging::config& config_;
    const std::vector<spdlog::sink_ptr> sinks_;
    const log_level level_;
};

// main initialize function for logging
void init(const logging::config& conf);

// creates a logger to be written to sink
// if a name already exist, it will return the existing logger
spdlog_manager::spdlog_module create_logger(std::string_view logger_name);

} // namespace fntr::logging

#define LOG_IMPL(logger, level, ...)                                                                                   \
        do {                                                                                                           \
            using namespace fntr::logging;                                                                             \
            if (spdlog_manager::should_log(level))                                                                     \
                spdlog_manager::log_message(logger, level, std::format(__VA_ARGS__));                                  \
    } while (0)

#define M_LOG_IMPL(log_module, level, ...)                                                                             \
        do {                                                                                                           \
            using namespace fntr::logging;                                                                             \
            if (spdlog_manager::should_log(log_module.name, level))                                                    \
                spdlog_manager::log_message(log_module.logger, level, std::format(__VA_ARGS__));                       \
    } while (0)

// use global logger
#define LOG_TRACE(...) LOG_IMPL(spdlog_manager::get_logger("fntr").logger, fntr::logging::log_level::trace, __VA_ARGS__)
#define LOG_DEBUG(...) LOG_IMPL(spdlog_manager::get_logger("fntr").logger, fntr::logging::log_level::debug, __VA_ARGS__)
#define LOG_INFO(...)  LOG_IMPL(spdlog_manager::get_logger("fntr").logger, fntr::logging::log_level::info, __VA_ARGS__)
#define LOG_WARN(...)  LOG_IMPL(spdlog_manager::get_logger("fntr").logger, fntr::logging::log_level::warn, __VA_ARGS__)
#define LOG_ERROR(...) LOG_IMPL(spdlog_manager::get_logger("fntr").logger, fntr::logging::log_level::error, __VA_ARGS__)
#define LOG_CRITICAL(...)                                                                                              \
    LOG_IMPL(spdlog_manager::get_logger("fntr").logger, fntr::logging::log_level::critical, __VA_ARGS__)

// use custom logger
#define M_LOG_TRACE(log_module, ...)    LOG_IMPL(log_module, fntr::logging::log_level::trace, __VA_ARGS__)
#define M_LOG_DEBUG(log_module, ...)    LOG_IMPL(log_module, fntr::logging::log_level::debug, __VA_ARGS__)
#define M_LOG_INFO(log_module, ...)     LOG_IMPL(log_module, fntr::logging::log_level::info, __VA_ARGS__)
#define M_LOG_WARN(log_module, ...)     LOG_IMPL(log_module, fntr::logging::log_level::warn, __VA_ARGS__)
#define M_LOG_ERROR(log_module, ...)    LOG_IMPL(log_module, fntr::logging::log_level::error, __VA_ARGS__)
#define M_LOG_CRITICAL(log_module, ...) LOG_IMPL(log_module, fntr::logging::log_level::critical, __VA_ARGS__)
