cmake_minimum_required(VERSION 3.18)
project(kstd LANGUAGES C CXX)

option(KSTD_BUILD_TESTS "Build unit tests for kstd" OFF)
option(KSTD_ENABLE_MOLD "Enable the mold linker on Unixoid systems if present" ON)
option(KSTD_GLOBAL_MIMALLOC "Enable the mimalloc allocator for new/delete operators" ON)

set(CMAKE_CXX_STANDARD 20) # Force C++20
set(CMAKE_C_STANDARD 17) # Force C17

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;")
include(kstd-platform)

# Library
file(GLOB_RECURSE KSTD_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/kstd/*.hpp")

add_library(kstd SHARED ${KSTD_SOURCE_FILES})
target_include_directories(kstd PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(kstd-static STATIC ${KSTD_SOURCE_FILES})
target_include_directories(kstd-static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Shared compiler definitions
if (KSTD_GLOBAL_MIMALLOC)
    target_compile_definitions(kstd PUBLIC KSTD_GLOBAL_MIMALLOC)
    target_compile_definitions(kstd-static PUBLIC KSTD_GLOBAL_MIMALLOC)
endif ()

if (KSTD_STACKTRACE)
    target_compile_definitions(kstd PUBLIC KSTD_STACKTRACE)
    target_compile_definitions(kstd-static PUBLIC KSTD_STACKTRACE)
endif ()

# Expose compiler identification information as compile definitions
if (KSTD_COMPILER_GCC)
    target_compile_definitions(kstd PUBLIC KSTD_COMPILER_GCC)
    target_compile_definitions(kstd-static PUBLIC KSTD_COMPILER_GCC)
elseif (KSTD_COMPILER_CLANG)
    target_compile_definitions(kstd PUBLIC KSTD_COMPILER_CLANG)
    target_compile_definitions(kstd-static PUBLIC KSTD_COMPILER_CLANG)
else ()
    target_compile_definitions(kstd PUBLIC KSTD_COMPILER_MSVC)
    target_compile_definitions(kstd-static PUBLIC KSTD_COMPILER_CLANG)
endif ()

# Expose platform identification information as compile definitions
if (KSTD_PLATFORM_MACOS)
    target_compile_definitions(kstd PUBLIC KSTD_PLATFORM_MACOS)
    target_compile_definitions(kstd-static PUBLIC KSTD_PLATFORM_MACOS)
endif ()
if (KSTD_PLATFORM_WINDOWS)
    target_compile_definitions(kstd PUBLIC KSTD_PLATFORM_WINDOWS)
    target_compile_definitions(kstd-static PUBLIC KSTD_PLATFORM_WINDOWS)
endif ()
if (KSTD_PLATFORM_LINUX)
    target_compile_definitions(kstd PUBLIC KSTD_PLATFORM_LINUX UNW_LOCAL_ONLY)
    target_compile_definitions(kstd-static PUBLIC KSTD_PLATFORM_LINUX UNW_LOCAL_ONLY)
    target_link_libraries(kstd PUBLIC -lunwind -ldwarf)
    target_link_libraries(kstd-static PUBLIC -lunwind -ldwarf)
endif ()
if (KSTD_PLATFORM_UNIX)
    target_compile_definitions(kstd PUBLIC KSTD_PLATFORM_UNIX UNW_LOCAL_ONLY)
    target_compile_definitions(kstd-static PUBLIC KSTD_PLATFORM_UNIX UNW_LOCAL_ONLY)
endif ()

# Enable mold if needed
if (KSTD_ENABLE_MOLD)
    find_program(KSTD_MOLD_EXECUTABLE "mold")
    if (KSTD_MOLD_EXECUTABLE AND NOT KSTD_COMPILER_CLANG) # mold doesn't work with Clang properly yet..
        message(STATUS "Detected mold linker, substituting")

        if (NOT DEFINED KSTD_NUM_LINK_THREADS) # So we can pass this in via CLI
            cmake_host_system_information(RESULT KSTD_NUM_LINK_THREADS QUERY NUMBER_OF_LOGICAL_CORES)
        endif () # NOT DEFINED NUM_LINK_THREADS

        message(STATUS "Using ${KSTD_NUM_LINK_THREADS} threads for linking")
        target_link_options(kstd PRIVATE "-fuse-ld=mold;LINKER:--threads,--thread-count=${KSTD_NUM_LINK_THREADS}")
        target_link_options(kstd-static PRIVATE "-fuse-ld=mold;LINKER:--threads,--thread-count=${KSTD_NUM_LINK_THREADS}")
    endif () # MOLD_EXECUTABLE AND NOT COMPILER_CLANG
endif ()

# libdwarf
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/libdwarf")
target_link_libraries(kstd PRIVATE libdwarf::dwarf-static)
target_link_libraries(kstd-static PRIVATE libdwarf::dwarf-static)

# mimalloc memory allocator
set(MI_BUILD_STATIC ON) # Make sure we enable the static target
set(MI_OVERRIDE OFF) # We don't want to replace standard malloc calls
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/mimalloc")
target_link_libraries(kstd PRIVATE mimalloc-static)
target_link_libraries(kstd-static PRIVATE mimalloc-static)

# fmt formatting library
set(FMT_EXCEPTIONS OFF)
set(FMT_NOEXCEPT ON)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/fmt")
target_link_libraries(kstd PRIVATE fmt::fmt)
target_link_libraries(kstd-static PRIVATE fmt::fmt)

# Tests
if (KSTD_BUILD_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest")
    include(GoogleTest)
    enable_testing(TRUE)
    file(GLOB_RECURSE KSTD_TEST_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp")
    add_executable(kstd-tests ${KSTD_TEST_SOURCE_FILES})
    target_link_libraries(kstd-tests PRIVATE gtest_main kstd-static)
    gtest_discover_tests(kstd-tests)

    if (KSTD_COMPILER_GCC OR KSTD_COMPILER_CLANG)
        target_compile_options(kstd-tests PRIVATE -g3 -gdwarf-5 -O0 -fno-inline -D_GLIBCXX_DEBUG_BACKTRACE)
    endif ()

    if (KSTD_BUILD_DEBUG AND KSTD_COMPILER_MSVC)
        target_compile_definitions(kstd-tests PRIVATE _ITERATOR_DEBUG_LEVEL=2) # Enable checked iterators
    endif ()
endif ()

# Enable debug options/definitions
if (KSTD_BUILD_DEBUG)
    if (KSTD_COMPILER_GCC OR KSTD_COMPILER_CLANG)
        target_compile_options(kstd PRIVATE -g3 -gdwarf-5 -O0 -fno-inline -D_GLIBCXX_DEBUG_BACKTRACE)
        target_compile_options(kstd-static PRIVATE -g3 -gdwarf-5 -O0 -fno-inline -D_GLIBCXX_DEBUG_BACKTRACE)
    endif ()
    if (KSTD_COMPILER_MSVC)
        target_compile_definitions(kstd PRIVATE _ITERATOR_DEBUG_LEVEL=2) # Enable checked iterators
        target_compile_definitions(kstd-static PRIVATE _ITERATOR_DEBUG_LEVEL=2) # Enable checked iterators
    endif ()
endif ()
