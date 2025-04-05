add_library(fntr_logging STATIC log.cpp)
common_compile_options(fntr_logging)
target_link_libraries(fntr_logging PUBLIC fntr_interface)
