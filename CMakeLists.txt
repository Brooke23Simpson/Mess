cmake_minimum_required(VERSION 3.12)
project(TypeAheadProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Main application source files
set(SOURCE_FILES
    main.cpp           # Main entry point for application
    WordTree.cpp       # WordTree implementation
    rlutil.h           # Console UI utility
)

# Dictionary file to be used at runtime
set(DICTIONARY_FILE dictionary.txt)

# Create executable for the main application
add_executable(TypeAhead ${SOURCE_FILES})

# Include directories for main application
target_include_directories(TypeAhead PRIVATE ${CMAKE_SOURCE_DIR})

# Set compile options based on the compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(TypeAhead PRIVATE /W4 /permissive-)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(TypeAhead PRIVATE -Wall -Wextra -pedantic -O3)
endif()

# Copy dictionary.txt to the build directory at runtime
add_custom_command(
    TARGET TypeAhead POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${DICTIONARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/dictionary.txt
)

# Set up GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Unit test files
set(TEST_FILES
    TestWordTree.cpp    # Unit tests for WordTree
    WordTree.cpp        # Shared implementation with main application
)

# Create UnitTestRunner target
add_executable(UnitTestRunner ${TEST_FILES})

# Link GoogleTest to UnitTestRunner
target_link_libraries(UnitTestRunner gtest_main)
if (UNIX)
    target_link_libraries(UnitTestRunner pthread)
endif()

# Include directories for UnitTestRunner
target_include_directories(UnitTestRunner PRIVATE ${CMAKE_SOURCE_DIR})

# Add GoogleTest to CMake
enable_testing()

# Register UnitTestRunner as a test
add_test(NAME UnitTestRunner COMMAND UnitTestRunner)

# Optional: Add clang-format target
find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
    unset(SOURCE_FILES_PATHS)
    foreach(SOURCE_FILE ${SOURCE_FILES})
        get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
        set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
    endforeach()
    add_custom_target(
        ClangFormat
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${SOURCE_FILES_PATHS}
    )
    add_dependencies(TypeAhead ClangFormat)
endif()
