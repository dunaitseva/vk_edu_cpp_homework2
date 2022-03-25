cmake_minimum_required(VERSION 3.16)
project(HW2)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")

set(MATRIX_LIB_NAME matrix)
set(INCLUDE project)
set(SOURCE project/src)

option(PARALLEL "Enable parallel implementation build" OFF)
if (PARALLEL)
    message(STATUS "Enabled parallel library implementation build")
    set(LIBRARY_TYPE SHARED)
    set()
else ()
    message(STATUS "Enabled consistent library implementation build (by default)")
    set(LIBRARY_TYPE STATIC)
endif ()

add_library(${MATRIX_LIB_NAME} ${LIBRARY_TYPE} ${SOURCE}/matrix.c)
target_include_directories(${MATRIX_LIB_NAME} PUBLIC ${INCLUDE})

option(TEST "Enable tests build" OFF)
if (TEST)
    set(CMAKE_CXX_STANDARD 17)
    set(TEST_DIR tests)

    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    include(GoogleTest)

    file(GLOB TestsSrc "${TEST_DIR}/*.cpp")
    foreach (file ${TestsSrc})
        message(${file})
        get_filename_component(target_test ${file} NAME_WE)

        add_executable(${target_test} ${file})
        target_link_libraries(${target_test} ${MATRIX_LIB_NAME})
        target_link_libraries(${target_test} gtest_main)
        gtest_discover_tests(${target_test})

        add_executable(${target_test}Address ${file})
        target_link_libraries(${target_test}Address ${MATRIX_LIB_NAME})
        target_link_options(${target_test}Address PRIVATE "-fsanitize=address" "-O0")
        target_link_libraries(${target_test}Address "-fsanitize=address")
        target_link_libraries(${target_test}Address gtest_main)
        gtest_discover_tests(${target_test}Address)

        add_executable(${target_test}Thread ${file})
        target_link_libraries(${target_test}Thread ${MATRIX_LIB_NAME})
        target_link_options(${target_test}Thread PRIVATE "-fsanitize=thread" "-O0")
        target_link_libraries(${target_test}Thread gtest_main)
        gtest_discover_tests(${target_test}Thread)

        add_executable(${target_test}Undefined ${file})
        target_link_libraries(${target_test}Undefined ${MATRIX_LIB_NAME})
        target_link_options(${target_test}Undefined PRIVATE "-fsanitize=undefined" "-O0")
        target_link_libraries(${target_test}Undefined gtest_main)
        gtest_discover_tests(${target_test}Undefined)

        message(STATUS "Set of ${target_test} tests builded")
    endforeach ()

    message(STATUS "Tests successfully builded")
endif ()
