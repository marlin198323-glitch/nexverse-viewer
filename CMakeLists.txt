cmake_minimum_required(VERSION 3.25)

# Some autotools-based vcpkg ports on Unix do not reliably handle an
# installation prefix containing spaces. Keep the vcpkg installation tree
# outside the source/build directory so paths such as "OGL Viewer" remain
# supported.
if(UNIX AND DEFINED ENV{HOME} AND NOT DEFINED VCPKG_INSTALLED_DIR)
    set(
        VCPKG_INSTALLED_DIR
        "$ENV{HOME}/.cache/nexverse-viewer/vcpkg_installed"
        CACHE PATH
        "External vcpkg install tree for NexVerse Viewer"
    )
endif()

project(NexVerseViewer VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" NEXVERSE_VERSION_LABEL)
string(STRIP "${NEXVERSE_VERSION_LABEL}" NEXVERSE_VERSION_LABEL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NEXVERSE_BUILD_TESTS "Build OpenGenesisLINK Viewer tests" ON)
option(NEXVERSE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

find_package(CURL REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Freetype REQUIRED)

add_library(nexverse-core
    src/core/logger.cpp
    src/core/version.cpp
)
add_library(NexVerse::Core ALIAS nexverse-core)

target_include_directories(nexverse-core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(nexverse-core PUBLIC
    NEXVERSE_VERSION="${NEXVERSE_VERSION_LABEL}"
)

add_library(nexverse-net
    src/net/curl_http_transport.cpp
    src/net/udp_socket.cpp
)
add_library(NexVerse::Net ALIAS nexverse-net)
target_include_directories(nexverse-net PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(nexverse-net PUBLIC NexVerse::Core CURL::libcurl)

add_library(nexverse-protocol
    src/protocol/grid_info.cpp
    src/protocol/grid_discovery.cpp
    src/protocol/login.cpp
    src/protocol/lludp.cpp
    src/protocol/circuit.cpp
)
add_library(NexVerse::Protocol ALIAS nexverse-protocol)
target_include_directories(nexverse-protocol PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(nexverse-protocol PUBLIC NexVerse::Core NexVerse::Net tinyxml2::tinyxml2 OpenSSL::Crypto)

add_library(nexverse-ui
    src/ui/ui_renderer.cpp
    src/app/application.cpp
)
add_library(NexVerse::UI ALIAS nexverse-ui)

target_include_directories(nexverse-ui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(nexverse-ui PUBLIC
    NexVerse::Core
    NexVerse::Protocol
    OpenGL::GL
    glfw
    glad::glad
    Freetype::Freetype
)

target_link_libraries(nexverse-ui PRIVATE Threads::Threads)

target_compile_definitions(nexverse-ui PUBLIC GLFW_INCLUDE_NONE)

if(APPLE)
    target_compile_definitions(nexverse-ui PRIVATE GL_SILENCE_DEPRECATION)
endif()

function(nexverse_enable_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /Zc:__cplusplus)
        if(NEXVERSE_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wshadow
        )
        if(NEXVERSE_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

nexverse_enable_warnings(nexverse-core)
nexverse_enable_warnings(nexverse-net)
nexverse_enable_warnings(nexverse-protocol)
nexverse_enable_warnings(nexverse-ui)

add_executable(nexverse-viewer apps/viewer/main.cpp)
target_link_libraries(nexverse-viewer PRIVATE NexVerse::UI)
nexverse_enable_warnings(nexverse-viewer)

install(TARGETS nexverse-viewer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CTest)
if(BUILD_TESTING AND NEXVERSE_BUILD_TESTS)
    add_executable(nexverse-core-tests tests/core_tests.cpp)
    target_link_libraries(nexverse-core-tests PRIVATE NexVerse::Core)
    nexverse_enable_warnings(nexverse-core-tests)

    add_test(NAME nexverse-core-tests COMMAND nexverse-core-tests)

    add_executable(nexverse-grid-info-tests tests/grid_info_tests.cpp)
    target_link_libraries(nexverse-grid-info-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-grid-info-tests)
    add_test(NAME nexverse-grid-info-tests COMMAND nexverse-grid-info-tests)

    add_executable(nexverse-grid-discovery-tests tests/grid_discovery_tests.cpp)
    target_link_libraries(nexverse-grid-discovery-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-grid-discovery-tests)
    add_test(NAME nexverse-grid-discovery-tests COMMAND nexverse-grid-discovery-tests)

    add_executable(nexverse-login-tests tests/login_tests.cpp)
    target_link_libraries(nexverse-login-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-login-tests)
    add_test(NAME nexverse-login-tests COMMAND nexverse-login-tests)

    add_executable(nexverse-lludp-tests tests/lludp_tests.cpp)
    target_link_libraries(nexverse-lludp-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-lludp-tests)
    add_test(NAME nexverse-lludp-tests COMMAND nexverse-lludp-tests)

    add_test(NAME nexverse-viewer-version COMMAND nexverse-viewer --version)
endif()
