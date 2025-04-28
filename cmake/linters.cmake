function(enable_linters)
    add_custom_target(clang-tidy
        COMMAND /usr/bin/clang-tidy -config-file="${PROJECT_SOURCE_DIR}/.clang-tidy" -p "${CMAKE_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/src/finance_tracker/main.cpp" -extra-arg=-std=c++23
        DEPENDS match_engine
    )

    # add_custom_target(valgrind-run
    #     COMMAND /usr/bin/valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose "${CMAKE_BINARY_DIR}/bin/finance_tracker" -c "${PROJECT_SOURCE_DIR}/sample_input.txt"
    #     DEPENDS match_engine
    # )
endfunction()
