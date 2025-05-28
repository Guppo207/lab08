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
if(APPLE)
    set(DMG_BACKGROUND "${CMAKE_CURRENT_SOURCE_DIR}/preview.png")
    if(NOT EXISTS "${DMG_BACKGROUND}")
        message(WARNING "preview.png not found! DMG background will be disabled.")
        set(CPACK_DMG_BACKGROUND_IMAGE "")
    else()
        set(CPACK_DMG_BACKGROUND_IMAGE "${DMG_BACKGROUND}")
    endif()
endif()

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Guppo207")
set(CPACK_PACKAGE_CONTACT "kirill.sherstyuk06@mail.ru")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 0)

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Guppo207 <kirill.sherstyuk06@mail.ru>")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")  
endif()

if(WIN32)
    set(CPACK_GENERATOR "WIX")
    set(CPACK_WIX_UPGRADE_GUID "9F9F7A1-3C4D-4E2F-9B1A-0E1D2C3B4A5F")  
endif()

if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
endif()

include(InstallRequiredSystemLibraries)
include(CPack)
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
    
    add_subdirectory(googletest)
    include(GoogleTest)
endif()
add_subdirectory(banking)

if(BUILD_TESTS)
    add_executable(RunTest test.cpp)
    target_link_libraries(RunTest PRIVATE banking gtest_main)
    gtest_discover_tests(RunTest)
endif()
install(TARGETS RunTest DESTINATION bin)
install(DIRECTORY banking/ DESTINATION include/banking FILES_MATCHING PATTERN "*.h*")
