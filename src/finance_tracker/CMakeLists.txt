add_library(fntr_interface INTERFACE)
target_include_directories(fntr_interface INTERFACE "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(fntr_interface INTERFACE boost::boost yaml-cpp spdlog::spdlog)

add_subdirectory(logging)

add_executable(finance_app main.cpp)
common_compile_options(finance_app)
target_link_libraries(finance_app PRIVATE fntr_logging)
install(TARGETS finance_app COMPONENT runtime)
