cmake_minimum_required(VERSION 3.25)

project(NexVerseViewer VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" NEXVERSE_VERSION_LABEL)
string(STRIP "${NEXVERSE_VERSION_LABEL}" NEXVERSE_VERSION_LABEL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NEXVERSE_BUILD_TESTS "Build OpenGenesisLINK Viewer tests" ON)
option(NEXVERSE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

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

add_library(nexverse-ui
    src/ui/ui_renderer.cpp
    src/ui/login_model.cpp
    src/ui/login_screen.cpp
    src/app/application.cpp
)
add_library(NexVerse::UI ALIAS nexverse-ui)

target_include_directories(nexverse-ui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(nexverse-ui PUBLIC
    NexVerse::Core
    OpenGL::GL
    glfw
    glad::glad
    Freetype::Freetype
)

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

    add_executable(nexverse-login-model-tests tests/login_model_tests.cpp)
    target_link_libraries(nexverse-login-model-tests PRIVATE NexVerse::UI)
    nexverse_enable_warnings(nexverse-login-model-tests)
    add_test(NAME nexverse-login-model-tests COMMAND nexverse-login-model-tests)

    add_test(NAME nexverse-viewer-version COMMAND nexverse-viewer --version)
endif()
