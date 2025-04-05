#include <finance_tracker/logging/log.hpp>
#include <finance_tracker/utils/macros.hpp>


int main(void)
{
    using namespace fntr;
    logging::config config;
    config.stdout = true;
    config.level = logging::log_level::info;
    // config.daily_file = std::make_optional<logging::file_config>("test.%Y%m%d%H%M%S.log");

    logging::init(config);
    LOG_INFO("Hello World {}", 1);

    auto& test_logger = logging::spdlog_manager::create_logger("test");
    M_LOG_INFO(test_logger, "Test with Hello World");
}
