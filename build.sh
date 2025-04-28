#!/bin/bash

ulimit -s unlimited

show_usage() {
    echo "$0 [-r|--rebuild] [-d|--debug] [-t|--test] [-f|--format] [-l|--lint]"
    exit 0
}

die() {
    echo -e "$1"
    exit 1
}

command_parser() {
    POSITIONAL_ARGS=()

    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_usage
                ;;
            -r|--rebuild)
                REBUILD=1
                shift # past argument
                ;;
            -d|--debug)
                CONFIG="Debug"
                shift # past argument
                ;;
            -t|--test)
                BUILD_TESTS=1
                shift # past argument
                ;;
            -f|--format)
                FORMATTING=1
                shift # past argument
                ;;
            -l|--lint)
                LINTING=1
                shift # past argument
                ;;
            -*|--*)
                die "Unknown option $1"
                ;;
            *)
                POSITIONAL_ARGS+=("$1") # Save positional arguments
                shift # past argument
                ;;
        esac
    done

    set -- "${POSITIONAL_ARGS[@]}" # restore positional parameters
}

main() {
    local APP_DATA_DIR=$(dirname "$(readlink -f "$0")")

    # Default arguments
    CONFIG="RelWithDebInfo"
    BUILD_DIR=$APP_DATA_DIR/.build
    REBUILD=0
    BUILD_TESTS=0
    FORMATTING=0
    LINTING=0

    command_parser $@

    if [ $LINTING -eq 1 ]; then
        CONFIG="Debug"
        REBUILD=1
    fi

    if [ $REBUILD -eq 1 ]; then
        echo "Deleting $BUILD_DIR ..."
        rm -rf $BUILD_DIR
    fi

    local EXTRA_BUILD_ARGS=
    if [ $BUILD_TESTS -eq 1 ]; then
        EXTRA_BUILD_ARGS+="-DBUILD_TESTS=ON "
    fi

    # Requirements prior to using conan:
    # Python >= 3.6
    # pip install conan
    # https://docs.conan.io/2/introduction.html
    conan install . --build=missing --output-folder=$BUILD_DIR --settings=build_type=$CONFIG
    local CONAN_CMAKE_TOOLCHAIN=$BUILD_DIR/build/$CONFIG/generators/conan_toolchain.cmake

    pushd $BUILD_DIR > /dev/null
    cmake .. -GNinja -DCMAKE_TOOLCHAIN_FILE=$CONAN_CMAKE_TOOLCHAIN -DCMAKE_BUILD_TYPE=$CONFIG $EXTRA_BUILD_ARGS

    # only format and exit
    if [ $FORMATTING -eq 1 ]; then
        ninja clang-format
        exit 0
    fi

    # only lint / memory checks etc. and exit
    if [ $LINTING -eq 1 ]; then
        ninja clang-tidy
        # ninja valgrind-run
        exit 0
    fi

    ninja
    local SUCCESS=$?

    if [ $SUCCESS -eq 0 ] && [ $BUILD_TESTS -eq 1 ]; then
        pushd $BUILD_DIR/tests > /dev/null

        echo "Build test detected, running unit tests"
        GTEST_COLOR=1 ctest --verbose

        popd > /dev/null
    fi

    popd > /dev/null
}

main $@
