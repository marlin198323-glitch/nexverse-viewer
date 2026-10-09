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
find_package(PNG REQUIRED)
find_package(OpenJPEG CONFIG REQUIRED)

if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WEBKITGTK REQUIRED IMPORTED_TARGET webkit2gtk-4.1)
endif()

add_library(nexverse-core
    src/core/logger.cpp
    src/core/system_metrics.cpp
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

add_library(nexverse-modules
    src/modules/manifest.cpp
    src/modules/module_manager.cpp
)
add_library(NexVerse::Modules ALIAS nexverse-modules)
target_include_directories(nexverse-modules PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(nexverse-modules PUBLIC NexVerse::Core PRIVATE ${CMAKE_DL_LIBS})

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

add_library(nexverse-assets
    src/assets/texture_loader.cpp
    src/assets/jpeg2000_decoder.cpp
    src/assets/mesh_loader.cpp
)
add_library(NexVerse::Assets ALIAS nexverse-assets)
target_include_directories(nexverse-assets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(nexverse-assets PUBLIC
    NexVerse::Core
    NexVerse::Net
    PNG::PNG
    openjp2
    NexVerse::Protocol
    ZLIB::ZLIB
)

add_library(nexverse-protocol
    src/protocol/grid_info.cpp
    src/protocol/grid_discovery.cpp
    src/protocol/login.cpp
    src/protocol/lludp.cpp
    src/protocol/circuit.cpp
    src/protocol/packet_dispatcher.cpp
    src/protocol/viewer_messages.cpp
    src/protocol/outbound_queue.cpp
    src/protocol/llsd.cpp
    src/protocol/terrain_codec.cpp
    src/protocol/caps.cpp
    src/protocol/caps_event_dispatcher.cpp
    src/protocol/caps_world_events.cpp
    src/protocol/environment_settings.cpp
    src/protocol/texture_entry.cpp
    src/protocol/render_materials.cpp
)
add_library(NexVerse::Protocol ALIAS nexverse-protocol)
target_include_directories(nexverse-protocol PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
find_package(ZLIB REQUIRED)
target_link_libraries(nexverse-protocol PUBLIC NexVerse::Core NexVerse::Net tinyxml2::tinyxml2 OpenSSL::Crypto ZLIB::ZLIB)

add_library(nexverse-world
    src/world/world_model.cpp
)
add_library(NexVerse::World ALIAS nexverse-world)
target_include_directories(nexverse-world PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(nexverse-world PUBLIC NexVerse::Protocol)

add_library(nexverse-render
    src/render/opengl_renderer.cpp
    src/render/terrain_mesh.cpp
    src/render/material_pass.cpp
)
add_library(NexVerse::Render ALIAS nexverse-render)
target_include_directories(nexverse-render PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(nexverse-render PUBLIC
    NexVerse::Core
    NexVerse::World
    OpenGL::GL
    glad::glad
)
target_compile_definitions(nexverse-render PUBLIC GLFW_INCLUDE_NONE)

add_library(nexverse-ui
    src/ui/ui_renderer.cpp
    src/ui/welcome_page.cpp
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
    NexVerse::World
    NexVerse::Render
    NexVerse::Assets
    NexVerse::Modules
    OpenGL::GL
    glfw
    glad::glad
    Freetype::Freetype
)

target_link_libraries(nexverse-ui PRIVATE Threads::Threads)

if(UNIX AND NOT APPLE)
    target_link_libraries(nexverse-ui PRIVATE PkgConfig::WEBKITGTK)
endif()

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
nexverse_enable_warnings(nexverse-modules)
nexverse_enable_warnings(nexverse-assets)
nexverse_enable_warnings(nexverse-protocol)
nexverse_enable_warnings(nexverse-world)
nexverse_enable_warnings(nexverse-render)
nexverse_enable_warnings(nexverse-ui)

add_executable(nexverse-viewer apps/viewer/main.cpp)
target_link_libraries(nexverse-viewer PRIVATE NexVerse::UI)
nexverse_enable_warnings(nexverse-viewer)

install(TARGETS nexverse-viewer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# The example module is a local developer-only test plugin, not installed by default.
if(WIN32)
    set(OGL_EXAMPLE_PLATFORM "windows")
elseif(APPLE)
    set(OGL_EXAMPLE_PLATFORM "macos")
else()
    set(OGL_EXAMPLE_PLATFORM "linux")
endif()
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
    set(OGL_EXAMPLE_ARCH "arm64")
else()
    set(OGL_EXAMPLE_ARCH "x64")
endif()
set(OGL_EXAMPLE_DIR "${CMAKE_BINARY_DIR}/modules/org.opengenesislink.example")
file(MAKE_DIRECTORY "${OGL_EXAMPLE_DIR}")
configure_file(modules/example/manifest.json.in "${OGL_EXAMPLE_DIR}/manifest.json" @ONLY)
add_library(nexverse-example-module MODULE modules/example/example_module.cpp)
target_include_directories(nexverse-example-module PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(nexverse-example-module PROPERTIES
    PREFIX ""
    OUTPUT_NAME "ogl_example_module"
    LIBRARY_OUTPUT_DIRECTORY "${OGL_EXAMPLE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${OGL_EXAMPLE_DIR}"
)
nexverse_enable_warnings(nexverse-example-module)

# ABI v2 developer diagnostics modules: build-only, never auto-installed.
foreach(module_name IN ITEMS developer_tools performance_monitor)
    if(module_name STREQUAL "developer_tools")
        set(OGL_MODULE_ID "org.opengenesislink.developer-tools")
        set(OGL_MODULE_BINARY "ogl_developer_tools")
    else()
        set(OGL_MODULE_ID "org.opengenesislink.performance-monitor")
        set(OGL_MODULE_BINARY "ogl_performance_monitor")
    endif()
    set(OGL_MODULE_OUT "${CMAKE_BINARY_DIR}/modules/${OGL_MODULE_ID}")
    file(MAKE_DIRECTORY "${OGL_MODULE_OUT}")
    configure_file(
        "modules/${module_name}/manifest.json.in"
        "${OGL_MODULE_OUT}/manifest.json" @ONLY
    )
    add_library("nexverse-${module_name}" MODULE
        "modules/${module_name}/${module_name}.cpp")
    target_include_directories("nexverse-${module_name}" PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
    set_target_properties("nexverse-${module_name}" PROPERTIES
        PREFIX ""
        OUTPUT_NAME "${OGL_MODULE_BINARY}"
        LIBRARY_OUTPUT_DIRECTORY "${OGL_MODULE_OUT}"
        RUNTIME_OUTPUT_DIRECTORY "${OGL_MODULE_OUT}"
    )
    nexverse_enable_warnings("nexverse-${module_name}")
endforeach()

include(CTest)
if(BUILD_TESTING AND NEXVERSE_BUILD_TESTS)
    add_executable(nexverse-core-tests tests/core_tests.cpp)
    target_link_libraries(nexverse-core-tests PRIVATE NexVerse::Core)
    nexverse_enable_warnings(nexverse-core-tests)

    add_test(NAME nexverse-core-tests COMMAND nexverse-core-tests)

    add_executable(nexverse-module-runtime-tests tests/module_runtime_tests.cpp)
    target_link_libraries(nexverse-module-runtime-tests PRIVATE NexVerse::Modules)
    add_dependencies(nexverse-module-runtime-tests nexverse-example-module)
    nexverse_enable_warnings(nexverse-module-runtime-tests)
    add_test(NAME nexverse-module-runtime-tests
        COMMAND nexverse-module-runtime-tests "${OGL_EXAMPLE_DIR}")

    add_executable(nexverse-developer-modules-tests tests/developer_modules_tests.cpp)
    target_link_libraries(nexverse-developer-modules-tests PRIVATE NexVerse::Modules)
    add_dependencies(nexverse-developer-modules-tests
        nexverse-developer_tools nexverse-performance_monitor)
    nexverse_enable_warnings(nexverse-developer-modules-tests)
    add_test(NAME nexverse-developer-modules-tests
        COMMAND nexverse-developer-modules-tests "${CMAKE_BINARY_DIR}/modules")


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

    add_executable(nexverse-packet-dispatcher-tests tests/packet_dispatcher_tests.cpp)
    target_link_libraries(nexverse-packet-dispatcher-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-packet-dispatcher-tests)
    add_test(NAME nexverse-packet-dispatcher-tests COMMAND nexverse-packet-dispatcher-tests)

    add_executable(nexverse-viewer-messages-tests tests/viewer_messages_tests.cpp)
    target_link_libraries(nexverse-viewer-messages-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-viewer-messages-tests)
    add_test(NAME nexverse-viewer-messages-tests COMMAND nexverse-viewer-messages-tests)

    add_executable(nexverse-outbound-queue-tests tests/outbound_queue_tests.cpp)
    target_link_libraries(nexverse-outbound-queue-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-outbound-queue-tests)
    add_test(NAME nexverse-outbound-queue-tests COMMAND nexverse-outbound-queue-tests)

    add_executable(nexverse-llsd-tests tests/llsd_tests.cpp)
    target_link_libraries(nexverse-llsd-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-llsd-tests)
    add_test(NAME nexverse-llsd-tests COMMAND nexverse-llsd-tests)

    add_executable(nexverse-render-materials-tests tests/render_materials_tests.cpp)
    target_link_libraries(nexverse-render-materials-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-render-materials-tests)
    add_test(NAME nexverse-render-materials-tests COMMAND nexverse-render-materials-tests)

    add_executable(nexverse-caps-tests tests/caps_tests.cpp)
    target_link_libraries(nexverse-caps-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-caps-tests)
    add_test(NAME nexverse-caps-tests COMMAND nexverse-caps-tests)

    add_executable(nexverse-caps-event-dispatcher-tests tests/caps_event_dispatcher_tests.cpp)
    target_link_libraries(nexverse-caps-event-dispatcher-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-caps-event-dispatcher-tests)
    add_test(NAME nexverse-caps-event-dispatcher-tests COMMAND nexverse-caps-event-dispatcher-tests)

    add_executable(nexverse-world-model-tests tests/world_model_tests.cpp)
    target_link_libraries(nexverse-world-model-tests PRIVATE NexVerse::World)
    nexverse_enable_warnings(nexverse-world-model-tests)
    add_test(NAME nexverse-world-model-tests COMMAND nexverse-world-model-tests)

    add_executable(nexverse-terrain-codec-tests tests/terrain_codec_tests.cpp)
    target_link_libraries(nexverse-terrain-codec-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-terrain-codec-tests)
    add_test(NAME nexverse-terrain-codec-tests COMMAND nexverse-terrain-codec-tests)

    add_executable(nexverse-environment-settings-tests tests/environment_settings_tests.cpp)
    target_link_libraries(nexverse-environment-settings-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-environment-settings-tests)
    add_test(NAME nexverse-environment-settings-tests COMMAND nexverse-environment-settings-tests)

    add_executable(nexverse-texture-entry-tests tests/texture_entry_tests.cpp)
    target_link_libraries(nexverse-texture-entry-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-texture-entry-tests)
    add_test(NAME nexverse-texture-entry-tests COMMAND nexverse-texture-entry-tests)

    add_executable(nexverse-texture-loader-tests tests/texture_loader_tests.cpp)
    target_link_libraries(nexverse-texture-loader-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-texture-loader-tests)
    add_test(NAME nexverse-texture-loader-tests COMMAND nexverse-texture-loader-tests)

    add_executable(nexverse-mesh-loader-tests tests/mesh_loader_tests.cpp)
    target_link_libraries(nexverse-mesh-loader-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-mesh-loader-tests)
    add_test(NAME nexverse-mesh-loader-tests COMMAND nexverse-mesh-loader-tests)

    add_executable(nexverse-caps-world-events-tests tests/caps_world_events_tests.cpp)
    target_link_libraries(nexverse-caps-world-events-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-caps-world-events-tests)
    add_test(NAME nexverse-caps-world-events-tests COMMAND nexverse-caps-world-events-tests)

    add_executable(nexverse-material-pass-tests tests/material_pass_tests.cpp)
    target_link_libraries(nexverse-material-pass-tests PRIVATE NexVerse::Render)
    nexverse_enable_warnings(nexverse-material-pass-tests)
    add_test(NAME nexverse-material-pass-tests COMMAND nexverse-material-pass-tests)

    add_executable(nexverse-terrain-mesh-tests tests/terrain_mesh_tests.cpp)
    target_link_libraries(nexverse-terrain-mesh-tests PRIVATE NexVerse::Render)
    nexverse_enable_warnings(nexverse-terrain-mesh-tests)
    add_test(NAME nexverse-terrain-mesh-tests COMMAND nexverse-terrain-mesh-tests)

    add_test(NAME nexverse-viewer-version COMMAND nexverse-viewer --version)
endif()
