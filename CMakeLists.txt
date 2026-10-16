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

# Changes to VERSION require CMake reconfiguration, not merely re-linking.
# Ninja should never report the prior alpha number after VERSION changed.
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/VERSION")
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
    src/assets/texture_mip.cpp
    src/assets/texture_memory_cache.cpp
    src/assets/texture_disk_cache.cpp
    src/assets/jpeg2000_decoder.cpp
    src/assets/mesh_loader.cpp
    src/assets/firestorm_avatar_mesh.cpp
    src/assets/firestorm_avatar_rig.cpp
    src/assets/firestorm_texlayer_manifest.cpp
    src/assets/firestorm_tga.cpp
    src/assets/mesh_cache.cpp
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
    tinyxml2::tinyxml2
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
    src/protocol/firestorm_compat.cpp
    src/protocol/firestorm_region_read.cpp
    src/protocol/firestorm_experience_permissions.cpp
    src/protocol/firestorm_inventory_items.cpp
    src/protocol/firestorm_inventory_tree.cpp
    src/protocol/firestorm_inventory_events.cpp
    src/protocol/region_event_queue.cpp
    src/protocol/firestorm_outfit_wear_preflight.cpp
    src/protocol/firestorm_inventory_gallery.cpp
    src/protocol/firestorm_inventory_fetch.cpp
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
    src/ui/viewer_shell_render.cpp
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

    add_executable(nexverse-outfit-bake-preview-layout-tests
        tests/outfit_gallery_bake_preview_layout_tests.cpp)
    target_link_libraries(nexverse-outfit-bake-preview-layout-tests
        PRIVATE NexVerse::UI)
    nexverse_enable_warnings(nexverse-outfit-bake-preview-layout-tests)
    add_test(NAME nexverse-outfit-bake-preview-layout-tests
        COMMAND nexverse-outfit-bake-preview-layout-tests)

    add_executable(nexverse-avatar-bake-preview-tests
        tests/avatar_bake_preview_tests.cpp)
    target_link_libraries(nexverse-avatar-bake-preview-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-avatar-bake-preview-tests)
    add_test(NAME nexverse-avatar-bake-preview-tests
        COMMAND nexverse-avatar-bake-preview-tests)

    add_executable(nexverse-avatar-bake-mip-priority-tests
        tests/avatar_bake_mip_priority_tests.cpp)
    target_link_libraries(nexverse-avatar-bake-mip-priority-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-avatar-bake-mip-priority-tests)
    add_test(NAME nexverse-avatar-bake-mip-priority-tests
        COMMAND nexverse-avatar-bake-mip-priority-tests)

    add_executable(nexverse-avatar-body-uv-evidence-tests
        tests/avatar_body_uv_evidence_tests.cpp)
    target_link_libraries(nexverse-avatar-body-uv-evidence-tests
        PRIVATE NexVerse::Render)
    nexverse_enable_warnings(nexverse-avatar-body-uv-evidence-tests)
    add_test(NAME nexverse-avatar-body-uv-evidence-tests
        COMMAND nexverse-avatar-body-uv-evidence-tests)

    add_executable(nexverse-avatar-bake-stream-priority-tests
        tests/avatar_bake_stream_priority_tests.cpp)
    target_link_libraries(nexverse-avatar-bake-stream-priority-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-avatar-bake-stream-priority-tests)
    add_test(NAME nexverse-avatar-bake-stream-priority-tests
        COMMAND nexverse-avatar-bake-stream-priority-tests)

    add_executable(nexverse-avatar-bake-evidence-tests
        tests/avatar_bake_evidence_tests.cpp)
    target_link_libraries(nexverse-avatar-bake-evidence-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-avatar-bake-evidence-tests)
    add_test(NAME nexverse-avatar-bake-evidence-tests
        COMMAND nexverse-avatar-bake-evidence-tests)

    add_executable(nexverse-firestorm-bake-coverage-tests
        tests/firestorm_bake_coverage_tests.cpp)
    target_link_libraries(nexverse-firestorm-bake-coverage-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-firestorm-bake-coverage-tests)
    add_test(NAME nexverse-firestorm-bake-coverage-tests
        COMMAND nexverse-firestorm-bake-coverage-tests)

    add_executable(nexverse-firestorm-global-mask-tests
        tests/firestorm_global_mask_tests.cpp)
    target_link_libraries(nexverse-firestorm-global-mask-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-firestorm-global-mask-tests)
    add_test(NAME nexverse-firestorm-global-mask-tests
        COMMAND nexverse-firestorm-global-mask-tests)

    add_executable(nexverse-firestorm-texlayer-color-tests
        tests/firestorm_texlayer_color_tests.cpp)
    target_link_libraries(nexverse-firestorm-texlayer-color-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-firestorm-texlayer-color-tests)
    add_test(NAME nexverse-firestorm-texlayer-color-tests
        COMMAND nexverse-firestorm-texlayer-color-tests)

    add_executable(nexverse-firestorm-tga-mask-tests
        tests/firestorm_tga_mask_tests.cpp)
    target_link_libraries(nexverse-firestorm-tga-mask-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-firestorm-tga-mask-tests)
    add_test(NAME nexverse-firestorm-tga-mask-tests
        COMMAND nexverse-firestorm-tga-mask-tests)

    add_executable(nexverse-firestorm-texlayer-manifest-tests
        tests/firestorm_texlayer_manifest_tests.cpp)
    target_link_libraries(nexverse-firestorm-texlayer-manifest-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-firestorm-texlayer-manifest-tests)
    add_test(NAME nexverse-firestorm-texlayer-manifest-tests
        COMMAND nexverse-firestorm-texlayer-manifest-tests)

    add_executable(nexverse-firestorm-wearable-texture-plan-tests
        tests/firestorm_wearable_texture_plan_tests.cpp)
    target_link_libraries(nexverse-firestorm-wearable-texture-plan-tests
        PRIVATE NexVerse::Assets NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-wearable-texture-plan-tests)
    add_test(NAME nexverse-firestorm-wearable-texture-plan-tests
        COMMAND nexverse-firestorm-wearable-texture-plan-tests)

    add_executable(nexverse-firestorm-wearable-transfer-tests
        tests/firestorm_wearable_transfer_tests.cpp)
    target_link_libraries(nexverse-firestorm-wearable-transfer-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-wearable-transfer-tests)
    add_test(NAME nexverse-firestorm-wearable-transfer-tests
        COMMAND nexverse-firestorm-wearable-transfer-tests)

    add_executable(nexverse-firestorm-wearable-asset-tests
        tests/firestorm_wearable_asset_tests.cpp)
    target_link_libraries(nexverse-firestorm-wearable-asset-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-wearable-asset-tests)
    add_test(NAME nexverse-firestorm-wearable-asset-tests
        COMMAND nexverse-firestorm-wearable-asset-tests)

    add_executable(nexverse-firestorm-wearable-slots-tests
        tests/firestorm_wearable_slots_tests.cpp)
    target_link_libraries(nexverse-firestorm-wearable-slots-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-wearable-slots-tests)
    add_test(NAME nexverse-firestorm-wearable-slots-tests
        COMMAND nexverse-firestorm-wearable-slots-tests)

    add_executable(nexverse-firestorm-appearance-verification-tests
        tests/firestorm_appearance_verification_tests.cpp)
    target_link_libraries(nexverse-firestorm-appearance-verification-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-appearance-verification-tests)
    add_test(NAME nexverse-firestorm-appearance-verification-tests
        COMMAND nexverse-firestorm-appearance-verification-tests)

    add_executable(nexverse-firestorm-cof-batch-tests
        tests/firestorm_cof_batch_tests.cpp)
    target_link_libraries(nexverse-firestorm-cof-batch-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-cof-batch-tests)
    add_test(NAME nexverse-firestorm-cof-batch-tests
        COMMAND nexverse-firestorm-cof-batch-tests)

    add_executable(nexverse-firestorm-cof-unlink-tests
        tests/firestorm_cof_unlink_tests.cpp)
    target_link_libraries(nexverse-firestorm-cof-unlink-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-cof-unlink-tests)
    add_test(NAME nexverse-firestorm-cof-unlink-tests
        COMMAND nexverse-firestorm-cof-unlink-tests)

    add_executable(nexverse-firestorm-cof-link-consent-tests
        tests/firestorm_cof_link_consent_tests.cpp)
    target_link_libraries(nexverse-firestorm-cof-link-consent-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-cof-link-consent-tests)
    add_test(NAME nexverse-firestorm-cof-link-consent-tests
        COMMAND nexverse-firestorm-cof-link-consent-tests)

    add_executable(nexverse-firestorm-cof-planning-tests
        tests/firestorm_cof_planning_tests.cpp)
    target_link_libraries(nexverse-firestorm-cof-planning-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-cof-planning-tests)
    add_test(NAME nexverse-firestorm-cof-planning-tests
        COMMAND nexverse-firestorm-cof-planning-tests)

    add_executable(nexverse-firestorm-inventory-tree-wear-tests
        tests/firestorm_inventory_tree_wear_tests.cpp)
    target_link_libraries(nexverse-firestorm-inventory-tree-wear-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-inventory-tree-wear-tests)
    add_test(NAME nexverse-firestorm-inventory-tree-wear-tests
        COMMAND nexverse-firestorm-inventory-tree-wear-tests)

    add_executable(nexverse-region-circuit-handoff-tests
        tests/region_circuit_handoff_tests.cpp)
    target_link_libraries(nexverse-region-circuit-handoff-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-region-circuit-handoff-tests)
    add_test(NAME nexverse-region-circuit-handoff-tests
        COMMAND nexverse-region-circuit-handoff-tests)

    add_executable(nexverse-region-event-queue-tests
        tests/region_event_queue_tests.cpp)
    target_link_libraries(nexverse-region-event-queue-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-region-event-queue-tests)
    add_test(NAME nexverse-region-event-queue-tests
        COMMAND nexverse-region-event-queue-tests)

    add_executable(nexverse-firestorm-inventory-events-tests
        tests/firestorm_inventory_events_tests.cpp)
    target_link_libraries(nexverse-firestorm-inventory-events-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-inventory-events-tests)
    add_test(NAME nexverse-firestorm-inventory-events-tests
        COMMAND nexverse-firestorm-inventory-events-tests)

    add_executable(nexverse-firestorm-inventory-items-tests
        tests/firestorm_inventory_items_tests.cpp)
    target_link_libraries(nexverse-firestorm-inventory-items-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-inventory-items-tests)
    add_test(NAME nexverse-firestorm-inventory-items-tests
        COMMAND nexverse-firestorm-inventory-items-tests)

    add_executable(nexverse-firestorm-inventory-sync-tests
        tests/firestorm_inventory_sync_tests.cpp)
    target_link_libraries(nexverse-firestorm-inventory-sync-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-inventory-sync-tests)
    add_test(NAME nexverse-firestorm-inventory-sync-tests
        COMMAND nexverse-firestorm-inventory-sync-tests)

    add_executable(nexverse-firestorm-experience-permissions-tests
        tests/firestorm_experience_permissions_tests.cpp)
    target_link_libraries(nexverse-firestorm-experience-permissions-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-experience-permissions-tests)
    add_test(NAME nexverse-firestorm-experience-permissions-tests
        COMMAND nexverse-firestorm-experience-permissions-tests)

    add_executable(nexverse-firestorm-inventory-fetch-tests
        tests/firestorm_inventory_fetch_tests.cpp)
    target_link_libraries(nexverse-firestorm-inventory-fetch-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-inventory-fetch-tests)
    add_test(NAME nexverse-firestorm-inventory-fetch-tests
        COMMAND nexverse-firestorm-inventory-fetch-tests)

    add_executable(nexverse-firestorm-inventory-gallery-tests
        tests/firestorm_inventory_gallery_tests.cpp)
    target_link_libraries(nexverse-firestorm-inventory-gallery-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-inventory-gallery-tests)
    add_test(NAME nexverse-firestorm-inventory-gallery-tests
        COMMAND nexverse-firestorm-inventory-gallery-tests)

    add_executable(nexverse-firestorm-region-read-tests
        tests/firestorm_region_read_tests.cpp)
    target_link_libraries(nexverse-firestorm-region-read-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-region-read-tests)
    add_test(NAME nexverse-firestorm-region-read-tests
        COMMAND nexverse-firestorm-region-read-tests)

    add_executable(nexverse-firestorm-region-lease-tests
        tests/firestorm_region_lease_tests.cpp)
    target_link_libraries(nexverse-firestorm-region-lease-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-region-lease-tests)
    add_test(NAME nexverse-firestorm-region-lease-tests
        COMMAND nexverse-firestorm-region-lease-tests)

    add_executable(nexverse-firestorm-thumbnail-metadata-tests
        tests/firestorm_thumbnail_metadata_tests.cpp)
    target_link_libraries(nexverse-firestorm-thumbnail-metadata-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-thumbnail-metadata-tests)
    add_test(NAME nexverse-firestorm-thumbnail-metadata-tests
        COMMAND nexverse-firestorm-thumbnail-metadata-tests)

    add_executable(nexverse-firestorm-compat-tests tests/firestorm_compat_tests.cpp)
    target_link_libraries(nexverse-firestorm-compat-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-firestorm-compat-tests)
    add_test(NAME nexverse-firestorm-compat-tests COMMAND nexverse-firestorm-compat-tests)

    add_executable(nexverse-caps-tests tests/caps_tests.cpp)
    target_link_libraries(nexverse-caps-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-caps-tests)
    add_test(NAME nexverse-caps-tests COMMAND nexverse-caps-tests)

    add_executable(nexverse-caps-event-dispatcher-tests tests/caps_event_dispatcher_tests.cpp)
    target_link_libraries(nexverse-caps-event-dispatcher-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-caps-event-dispatcher-tests)
    add_test(NAME nexverse-caps-event-dispatcher-tests COMMAND nexverse-caps-event-dispatcher-tests)

    add_executable(nexverse-avatar-animation-tests
        tests/avatar_animation_tests.cpp)
    target_link_libraries(nexverse-avatar-animation-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-avatar-animation-tests)
    add_test(NAME nexverse-avatar-animation-tests
        COMMAND nexverse-avatar-animation-tests)

    add_executable(nexverse-world-object-picker-tests
        tests/world_object_picker_tests.cpp)
    target_link_libraries(nexverse-world-object-picker-tests
        PRIVATE NexVerse::Render)
    nexverse_enable_warnings(nexverse-world-object-picker-tests)
    add_test(NAME nexverse-world-object-picker-tests
        COMMAND nexverse-world-object-picker-tests)

    add_executable(nexverse-object-touch-tests
        tests/object_touch_tests.cpp)
    target_link_libraries(nexverse-object-touch-tests
        PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-object-touch-tests)
    add_test(NAME nexverse-object-touch-tests
        COMMAND nexverse-object-touch-tests)

    add_executable(nexverse-animation-asset-transfer-tests
        tests/animation_asset_transfer_tests.cpp)
    target_link_libraries(nexverse-animation-asset-transfer-tests
        PRIVATE NexVerse::Protocol NexVerse::Assets)
    nexverse_enable_warnings(nexverse-animation-asset-transfer-tests)
    add_test(NAME nexverse-animation-asset-transfer-tests
        COMMAND nexverse-animation-asset-transfer-tests)

    add_executable(nexverse-firestorm-keyframe-motion-tests
        tests/firestorm_keyframe_motion_tests.cpp)
    target_link_libraries(nexverse-firestorm-keyframe-motion-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-firestorm-keyframe-motion-tests)
    add_test(NAME nexverse-firestorm-keyframe-motion-tests
        COMMAND nexverse-firestorm-keyframe-motion-tests)

    add_executable(nexverse-outfit-gallery-layout-tests
        tests/outfit_gallery_layout_tests.cpp)
    target_link_libraries(nexverse-outfit-gallery-layout-tests
        PRIVATE NexVerse::UI)
    nexverse_enable_warnings(nexverse-outfit-gallery-layout-tests)
    add_test(NAME nexverse-outfit-gallery-layout-tests
        COMMAND nexverse-outfit-gallery-layout-tests)

    add_executable(nexverse-viewer-shell-tests
        tests/viewer_shell_tests.cpp)
    target_link_libraries(nexverse-viewer-shell-tests
        PRIVATE NexVerse::UI)
    nexverse_enable_warnings(nexverse-viewer-shell-tests)
    add_test(NAME nexverse-viewer-shell-tests
        COMMAND nexverse-viewer-shell-tests)

    add_executable(nexverse-viewer-shell-gl-tests
        tests/viewer_shell_gl_tests.cpp)
    target_link_libraries(nexverse-viewer-shell-gl-tests
        PRIVATE NexVerse::UI glfw glad::glad)
    nexverse_enable_warnings(nexverse-viewer-shell-gl-tests)
    add_test(NAME nexverse-viewer-shell-gl-tests
        COMMAND nexverse-viewer-shell-gl-tests)
    set_tests_properties(nexverse-viewer-shell-gl-tests
        PROPERTIES SKIP_RETURN_CODE 77)

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

    add_executable(nexverse-firestorm-asset-locator-tests
        tests/firestorm_asset_locator_tests.cpp)
    target_link_libraries(nexverse-firestorm-asset-locator-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-firestorm-asset-locator-tests)
    add_test(NAME nexverse-firestorm-asset-locator-tests
        COMMAND nexverse-firestorm-asset-locator-tests)

    add_executable(nexverse-firestorm-avatar-mesh-tests
        tests/firestorm_avatar_mesh_tests.cpp)
    target_link_libraries(nexverse-firestorm-avatar-mesh-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-firestorm-avatar-mesh-tests)
    add_test(NAME nexverse-firestorm-avatar-mesh-tests
        COMMAND nexverse-firestorm-avatar-mesh-tests)
    add_executable(nexverse-mesh-loader-tests tests/mesh_loader_tests.cpp)
    target_link_libraries(nexverse-mesh-loader-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-mesh-loader-tests)
    add_test(NAME nexverse-mesh-loader-tests COMMAND nexverse-mesh-loader-tests)

    add_executable(nexverse-mesh-cache-tests tests/mesh_cache_tests.cpp)
    target_link_libraries(nexverse-mesh-cache-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-mesh-cache-tests)
    add_test(NAME nexverse-mesh-cache-tests COMMAND nexverse-mesh-cache-tests)

    add_executable(nexverse-mesh-lod-policy-tests tests/mesh_lod_policy_tests.cpp)
    target_link_libraries(nexverse-mesh-lod-policy-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-mesh-lod-policy-tests)
    add_test(NAME nexverse-mesh-lod-policy-tests COMMAND nexverse-mesh-lod-policy-tests)

    add_executable(nexverse-texture-prefetch-tests tests/texture_prefetch_tests.cpp)
    target_link_libraries(nexverse-texture-prefetch-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-texture-prefetch-tests)
    add_test(NAME nexverse-texture-prefetch-tests COMMAND nexverse-texture-prefetch-tests)

    add_executable(nexverse-gpu-staging-recovery-tests tests/gpu_staging_recovery_tests.cpp)
    target_link_libraries(nexverse-gpu-staging-recovery-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-gpu-staging-recovery-tests)
    add_test(NAME nexverse-gpu-staging-recovery-tests
        COMMAND nexverse-gpu-staging-recovery-tests)

    add_executable(nexverse-gpu-staging-budget-tests tests/gpu_staging_budget_tests.cpp)
    target_link_libraries(nexverse-gpu-staging-budget-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-gpu-staging-budget-tests)
    add_test(NAME nexverse-gpu-staging-budget-tests
        COMMAND nexverse-gpu-staging-budget-tests)

    add_executable(nexverse-asset-gpu-pressure-pipeline-tests tests/asset_gpu_pressure_pipeline_tests.cpp)
    target_link_libraries(nexverse-asset-gpu-pressure-pipeline-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-gpu-pressure-pipeline-tests)
    add_test(NAME nexverse-asset-gpu-pressure-pipeline-tests COMMAND nexverse-asset-gpu-pressure-pipeline-tests)

    add_executable(nexverse-texture-http-epoch-contract-tests
        tests/texture_http_epoch_contract_tests.cpp)
    target_link_libraries(nexverse-texture-http-epoch-contract-tests
        PRIVATE NexVerse::Assets NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-texture-http-epoch-contract-tests)
    add_test(NAME nexverse-texture-http-epoch-contract-tests
        COMMAND nexverse-texture-http-epoch-contract-tests)

    add_executable(nexverse-region-http-epoch-contract-tests
        tests/region_http_epoch_contract_tests.cpp)
    target_link_libraries(nexverse-region-http-epoch-contract-tests
        PRIVATE NexVerse::Assets NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-region-http-epoch-contract-tests)
    add_test(NAME nexverse-region-http-epoch-contract-tests
        COMMAND nexverse-region-http-epoch-contract-tests)

    add_executable(nexverse-asset-missing-id-counter-tests
        tests/asset_missing_id_counter_tests.cpp)
    target_link_libraries(nexverse-asset-missing-id-counter-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-missing-id-counter-tests)
    add_test(NAME nexverse-asset-missing-id-counter-tests
        COMMAND nexverse-asset-missing-id-counter-tests)

    add_executable(nexverse-asset-cache-capacity-profiles-tests
        tests/asset_cache_capacity_profiles_tests.cpp)
    target_link_libraries(nexverse-asset-cache-capacity-profiles-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-cache-capacity-profiles-tests)
    add_test(NAME nexverse-asset-cache-capacity-profiles-tests
        COMMAND nexverse-asset-cache-capacity-profiles-tests)

    add_executable(nexverse-asset-cache-pressure-lru-tests
        tests/asset_cache_pressure_lru_tests.cpp)
    target_link_libraries(nexverse-asset-cache-pressure-lru-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-cache-pressure-lru-tests)
    add_test(NAME nexverse-asset-cache-pressure-lru-tests
        COMMAND nexverse-asset-cache-pressure-lru-tests)

    add_executable(nexverse-asset-cache-pressure-window-tests
        tests/asset_cache_pressure_window_tests.cpp)
    target_link_libraries(nexverse-asset-cache-pressure-window-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-cache-pressure-window-tests)
    add_test(NAME nexverse-asset-cache-pressure-window-tests
        COMMAND nexverse-asset-cache-pressure-window-tests)

    add_executable(nexverse-asset-failure-telemetry-tests
        tests/asset_failure_telemetry_tests.cpp)
    target_link_libraries(nexverse-asset-failure-telemetry-tests
        PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-failure-telemetry-tests)
    add_test(NAME nexverse-asset-failure-telemetry-tests
        COMMAND nexverse-asset-failure-telemetry-tests)

    add_executable(nexverse-asset-reuse-history-tests tests/asset_reuse_history_tests.cpp)
    target_link_libraries(nexverse-asset-reuse-history-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-reuse-history-tests)
    add_test(NAME nexverse-asset-reuse-history-tests COMMAND nexverse-asset-reuse-history-tests)

    add_executable(nexverse-asset-full-residency-tests tests/asset_full_residency_tests.cpp)
    target_link_libraries(nexverse-asset-full-residency-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-full-residency-tests)
    add_test(NAME nexverse-asset-full-residency-tests COMMAND nexverse-asset-full-residency-tests)

    add_executable(nexverse-renderer-gpu-profiles-tests tests/renderer_gpu_profiles_tests.cpp)
    target_link_libraries(nexverse-renderer-gpu-profiles-tests PRIVATE NexVerse::Render glfw glad::glad)
    nexverse_enable_warnings(nexverse-renderer-gpu-profiles-tests)
    add_test(NAME nexverse-renderer-gpu-profiles-tests
        COMMAND nexverse-renderer-gpu-profiles-tests)
    set_tests_properties(nexverse-renderer-gpu-profiles-tests
        PROPERTIES SKIP_RETURN_CODE 77)

    add_executable(nexverse-renderer-alpha-framebuffer-tests
        tests/renderer_alpha_framebuffer_tests.cpp)
    target_link_libraries(nexverse-renderer-alpha-framebuffer-tests
        PRIVATE NexVerse::Render glfw glad::glad)
    nexverse_enable_warnings(nexverse-renderer-alpha-framebuffer-tests)
    add_test(NAME nexverse-renderer-alpha-framebuffer-tests
        COMMAND nexverse-renderer-alpha-framebuffer-tests)
    set_tests_properties(nexverse-renderer-alpha-framebuffer-tests
        PROPERTIES SKIP_RETURN_CODE 77)

    add_executable(nexverse-renderer-region-transition-tests
        tests/renderer_region_transition_tests.cpp)
    target_link_libraries(nexverse-renderer-region-transition-tests
        PRIVATE NexVerse::Render NexVerse::Assets glfw glad::glad)
    nexverse_enable_warnings(nexverse-renderer-region-transition-tests)
    add_test(NAME nexverse-renderer-region-transition-tests
        COMMAND nexverse-renderer-region-transition-tests)
    set_tests_properties(nexverse-renderer-region-transition-tests
        PROPERTIES SKIP_RETURN_CODE 77)

    add_executable(nexverse-renderer-gpu-soak-tests tests/renderer_gpu_soak_tests.cpp)
    target_link_libraries(nexverse-renderer-gpu-soak-tests PRIVATE NexVerse::Render NexVerse::Assets glfw glad::glad)
    nexverse_enable_warnings(nexverse-renderer-gpu-soak-tests)
    add_test(NAME nexverse-renderer-gpu-soak-tests COMMAND nexverse-renderer-gpu-soak-tests)
    set_tests_properties(nexverse-renderer-gpu-soak-tests PROPERTIES SKIP_RETURN_CODE 77)

    add_executable(nexverse-renderer-gpu-pressure-tests tests/renderer_gpu_pressure_tests.cpp)
    target_link_libraries(nexverse-renderer-gpu-pressure-tests PRIVATE NexVerse::Render glfw glad::glad)
    nexverse_enable_warnings(nexverse-renderer-gpu-pressure-tests)
    add_test(NAME nexverse-renderer-gpu-pressure-tests COMMAND nexverse-renderer-gpu-pressure-tests)
    set_tests_properties(nexverse-renderer-gpu-pressure-tests
        PROPERTIES SKIP_RETURN_CODE 77)

    add_executable(nexverse-asset-upload-telemetry-tests tests/asset_upload_telemetry_tests.cpp)
    target_link_libraries(nexverse-asset-upload-telemetry-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-upload-telemetry-tests)
    add_test(NAME nexverse-asset-upload-telemetry-tests COMMAND nexverse-asset-upload-telemetry-tests)

    add_executable(nexverse-asset-scene-priority-feedback-tests tests/asset_scene_priority_feedback_tests.cpp)
    target_link_libraries(nexverse-asset-scene-priority-feedback-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-scene-priority-feedback-tests)
    add_test(NAME nexverse-asset-scene-priority-feedback-tests COMMAND nexverse-asset-scene-priority-feedback-tests)

    add_executable(nexverse-renderer-gpu-asset-integration-tests tests/renderer_gpu_asset_integration_tests.cpp)
    target_link_libraries(nexverse-renderer-gpu-asset-integration-tests PRIVATE NexVerse::Render glfw glad::glad)
    nexverse_enable_warnings(nexverse-renderer-gpu-asset-integration-tests)
    add_test(NAME nexverse-renderer-gpu-asset-integration-tests
        COMMAND nexverse-renderer-gpu-asset-integration-tests)
    set_tests_properties(nexverse-renderer-gpu-asset-integration-tests
        PROPERTIES SKIP_RETURN_CODE 77)

    add_executable(nexverse-asset-lod-dwell-tests tests/asset_lod_dwell_tests.cpp)
    target_link_libraries(nexverse-asset-lod-dwell-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-lod-dwell-tests)
    add_test(NAME nexverse-asset-lod-dwell-tests COMMAND nexverse-asset-lod-dwell-tests)

    add_executable(nexverse-asset-gpu-upload-gate-tests tests/asset_gpu_upload_gate_tests.cpp)
    target_link_libraries(nexverse-asset-gpu-upload-gate-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-gpu-upload-gate-tests)
    add_test(NAME nexverse-asset-gpu-upload-gate-tests COMMAND nexverse-asset-gpu-upload-gate-tests)

    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME nexverse-asset-log-report-tests
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_ogl_asset_log_report.py")
        add_test(NAME nexverse-inworld-acceptance-tests
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_ogl_inworld_acceptance.py")
        add_test(NAME nexverse-inworld-transition-report-tests
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_ogl_inworld_transition_report.py")
    endif()

    add_executable(nexverse-asset-scene-coverage-tests tests/asset_scene_coverage_tests.cpp)
    target_link_libraries(nexverse-asset-scene-coverage-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-scene-coverage-tests)
    add_test(NAME nexverse-asset-scene-coverage-tests COMMAND nexverse-asset-scene-coverage-tests)

    add_executable(nexverse-asset-request-fairness-tests tests/asset_request_fairness_tests.cpp)
    target_link_libraries(nexverse-asset-request-fairness-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-request-fairness-tests)
    add_test(NAME nexverse-asset-request-fairness-tests COMMAND nexverse-asset-request-fairness-tests)

    add_executable(nexverse-asset-streaming-snapshot-tests tests/asset_streaming_snapshot_tests.cpp)
    target_link_libraries(nexverse-asset-streaming-snapshot-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-streaming-snapshot-tests)
    add_test(NAME nexverse-asset-streaming-snapshot-tests COMMAND nexverse-asset-streaming-snapshot-tests)

    add_executable(nexverse-asset-stream-integration-tests tests/asset_stream_integration_tests.cpp)
    target_link_libraries(nexverse-asset-stream-integration-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-stream-integration-tests)
    add_test(NAME nexverse-asset-stream-integration-tests COMMAND nexverse-asset-stream-integration-tests)

    add_executable(nexverse-asset-failure-tests tests/asset_failure_tests.cpp)
    target_link_libraries(nexverse-asset-failure-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-failure-tests)
    add_test(NAME nexverse-asset-failure-tests COMMAND nexverse-asset-failure-tests)

    add_executable(nexverse-asset-stream-scheduler-tests tests/asset_stream_scheduler_tests.cpp)
    target_link_libraries(nexverse-asset-stream-scheduler-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-stream-scheduler-tests)
    add_test(NAME nexverse-asset-stream-scheduler-tests COMMAND nexverse-asset-stream-scheduler-tests)

    add_executable(nexverse-asset-gpu-budget-tests tests/asset_gpu_budget_tests.cpp)
    target_link_libraries(nexverse-asset-gpu-budget-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-gpu-budget-tests)
    add_test(NAME nexverse-asset-gpu-budget-tests COMMAND nexverse-asset-gpu-budget-tests)

    add_executable(nexverse-asset-view-priority-tests tests/asset_view_priority_tests.cpp)
    target_link_libraries(nexverse-asset-view-priority-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-asset-view-priority-tests)
    add_test(NAME nexverse-asset-view-priority-tests COMMAND nexverse-asset-view-priority-tests)

    add_executable(nexverse-mesh-residency-tests tests/mesh_residency_tests.cpp)
    target_link_libraries(nexverse-mesh-residency-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-mesh-residency-tests)
    add_test(NAME nexverse-mesh-residency-tests COMMAND nexverse-mesh-residency-tests)

    add_executable(nexverse-texture-priority-tests tests/texture_priority_tests.cpp)
    target_link_libraries(nexverse-texture-priority-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-texture-priority-tests)
    add_test(NAME nexverse-texture-priority-tests COMMAND nexverse-texture-priority-tests)

    add_executable(nexverse-particle-motion-tests tests/particle_motion_tests.cpp)
    target_link_libraries(nexverse-particle-motion-tests PRIVATE NexVerse::Render)
    nexverse_enable_warnings(nexverse-particle-motion-tests)
    add_test(NAME nexverse-particle-motion-tests COMMAND nexverse-particle-motion-tests)

    add_executable(nexverse-particle-emission-tests tests/particle_emission_tests.cpp)
    target_link_libraries(nexverse-particle-emission-tests PRIVATE NexVerse::Render)
    nexverse_enable_warnings(nexverse-particle-emission-tests)
    add_test(NAME nexverse-particle-emission-tests COMMAND nexverse-particle-emission-tests)

    add_executable(nexverse-texture-residency-tests tests/texture_residency_tests.cpp)
    target_link_libraries(nexverse-texture-residency-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-texture-residency-tests)
    add_test(NAME nexverse-texture-residency-tests COMMAND nexverse-texture-residency-tests)

    add_executable(nexverse-texture-disk-cache-tests tests/texture_disk_cache_tests.cpp)
    target_link_libraries(nexverse-texture-disk-cache-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-texture-disk-cache-tests)
    add_test(NAME nexverse-texture-disk-cache-tests COMMAND nexverse-texture-disk-cache-tests)

    add_executable(nexverse-texture-memory-cache-tests tests/texture_memory_cache_tests.cpp)
    target_link_libraries(nexverse-texture-memory-cache-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-texture-memory-cache-tests)
    add_test(NAME nexverse-texture-memory-cache-tests COMMAND nexverse-texture-memory-cache-tests)

    add_executable(nexverse-texture-mip-tests tests/texture_mip_tests.cpp)
    target_link_libraries(nexverse-texture-mip-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-texture-mip-tests)
    add_test(NAME nexverse-texture-mip-tests COMMAND nexverse-texture-mip-tests)

    add_executable(nexverse-texture-distance-tests tests/texture_distance_tests.cpp)
    target_link_libraries(nexverse-texture-distance-tests PRIVATE NexVerse::Assets)
    nexverse_enable_warnings(nexverse-texture-distance-tests)
    add_test(NAME nexverse-texture-distance-tests COMMAND nexverse-texture-distance-tests)

    add_executable(nexverse-movement-controls-tests tests/movement_controls_tests.cpp)
    target_link_libraries(nexverse-movement-controls-tests PRIVATE NexVerse::Protocol)
    nexverse_enable_warnings(nexverse-movement-controls-tests)
    add_test(NAME nexverse-movement-controls-tests COMMAND nexverse-movement-controls-tests)

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
