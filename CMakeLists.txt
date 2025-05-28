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

add_subdirectory(banking)
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

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    add_subdirectory(googletest)
    add_executable(RunTest test.cpp)

    target_include_directories(RunTest PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/banking
        ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
        ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include
    )

    target_link_libraries(RunTest PRIVATE
        banking
        gtest
        gmock
        gtest_main
    )

    if(COVERAGE)
        target_compile_options(RunTest PRIVATE --coverage)
        target_link_options(RunTest PRIVATE --coverage)
    endif()
    include(GoogleTest)
    gtest_discover_tests(RunTest)
endif()
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Guppo207")
set(CPACK_PACKAGE_CONTACT "kirill.sherstyuk06@mail.ru")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

if(WIN32)
    set(CPACK_GENERATOR "WIX")
    set(CPACK_WIX_UPGRADE_GUID "PUT-GUID-HERE-1234-5678-90AB-CDEF12345678")
    set(CPACK_WIX_PROPERTY_ARGS "-dWixUILicenseEnabled=0")
    set(CPACK_WIX_SKIP_LICENSE_PAGE ON)
    set(CPACK_WIX_UI_REF "WixUI_InstallDir") 
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/preview.png")
        set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/preview.png")
    endif()
else()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Guppo207 <kirill.sherstyuk06@mail.ru>")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
endif()

include(InstallRequiredSystemLibraries)
include(CPack)
