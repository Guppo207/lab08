cmake_minimum_required(VERSION 3.10...3.28)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(COVERAGE "Enable code coverage" OFF)
option(BUILD_TESTS "Build tests" ON)

project(TestRunning
    VERSION 0.1.0
    DESCRIPTION "Banking application test framework"
    LANGUAGES CXX
)

if(BUILD_TESTS)
    find_package(Git REQUIRED)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
        message(STATUS "Cloning googletest submodule...")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT
        )
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to initialize submodules")
        endif()
    endif()
    
    set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    add_subdirectory(googletest)
    
    if(NOT TARGET GTest::GTest)
        add_library(GTest::GTest ALIAS gtest)
    endif()
    if(NOT TARGET GTest::Main)
        add_library(GTest::Main ALIAS gtest_main)
    endif()
    if(NOT TARGET GMock::Main)
        add_library(GMock::Main ALIAS gmock_main)
    endif()
    
    include(GoogleTest)
endif()

add_subdirectory(banking)

set(CPACK_PACKAGE_NAME "TestRunning")
set(CPACK_PACKAGE_VENDOR "Guppo207")
set(CPACK_PACKAGE_CONTACT "kirill.sherstyuk06@mail.ru")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

include(InstallRequiredSystemLibraries)
include(CPack)

if(BUILD_TESTS)
    add_executable(RunTest test.cpp)
    
    target_include_directories(RunTest PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/banking
    )
    
    target_link_libraries(RunTest PRIVATE 
        GTest::GTest
        GTest::Main
        GMock::Main
        banking
    )
    
    if(COVERAGE)
        target_compile_options(RunTest PRIVATE --coverage)
        target_link_options(RunTest PRIVATE --coverage)
    endif()

    gtest_discover_tests(RunTest)
endif()

install(TARGETS RunTest
    RUNTIME DESTINATION bin
    COMPONENT tests
)

install(DIRECTORY banking/
    DESTINATION include/banking
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN "*.hpp"
)
