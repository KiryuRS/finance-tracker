#include <finance_tracker/logging/detail/spdlog_impl.hpp>
#include <finance_tracker/logging/log.hpp>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <chrono>
#include <iostream>

namespace fntr::logging {

namespace {

std::vector<spdlog::sink_ptr> create_sinks(const logging::config& conf)
{
    using daily_file_sink_st =
        spdlog::sinks::daily_file_sink<spdlog::details::null_mutex, spdlog::sinks::daily_filename_format_calculator>;

    std::vector<spdlog::sink_ptr> sinkers;
    if (conf.stdout) sinkers.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());

        if (conf.daily_file) {
            // TODO: Handle symlink
            sinkers.push_back(std::make_shared<daily_file_sink_st>(conf.daily_file->filepath, 0, 0));
        }
    return sinkers;
}

} // namespace

spdlog_manager::spdlog_manager(const logging::config& conf)
    : sinks_{create_sinks(conf)},
      level_{conf.level}
{
    auto [iter, _] = loggers_.emplace("fntr", std::make_shared<spdlog::logger>("fntr", sinks_.begin(), sinks_.end()));
    auto& logger   = iter->second;

    auto pattern_formatter = std::make_unique<spdlog::pattern_formatter>();
    pattern_formatter->add_flag<detail::formatter::hostname_flag>('h').set_pattern(
        "[%Y-%m-%dT%H:%M:%S.%e][%P][%t][%h][%^%l%$][%n] %v"
    );
    logger->set_formatter(std::move(pattern_formatter));
    spdlog::set_level(static_cast<spdlog::level::level_enum>(conf.level));
    spdlog::register_logger(logger);
}

void spdlog_manager::log_message(spdlog::logger& logger, log_level level, std::string message)
{
    logger.log(static_cast<spdlog::level::level_enum>(level), message);
}

bool spdlog_manager::should_log(log_level level) { return global_logger->level_ <= level; }

spdlog::logger& spdlog_manager::create_logger(std::string_view logger_name)
{
    auto [iter, _] = global_logger->loggers_.emplace(
        logger_name, std::make_shared<spdlog::logger>(
                         std::string{logger_name}, global_logger->sinks_.begin(), global_logger->sinks_.end()
                     )
    );
    return *(iter->second);
}

spdlog::logger& spdlog_manager::get_logger(std::string_view logger_name)
{
    return *global_logger->loggers_.at(logger_name);
}

void init(const logging::config& conf)
{
    if (!spdlog_manager::global_logger) spdlog_manager::global_logger = std::make_unique<spdlog_manager>(conf);
}

spdlog::logger& create_logger(std::string_view logger_name) { return spdlog_manager::create_logger(logger_name); }

} // namespace fntr::logging
