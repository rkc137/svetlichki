cmake_minimum_required(VERSION 3.10)
project(svetlichki LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(FETCHCONTENT_UPDATES_DISCONNECTED OFF)
include(FetchContent)
if(DEFINED ENV{CMAKE_LIBS_DIR})
    set(FETCHCONTENT_BASE_DIR "$ENV{CMAKE_LIBS_DIR}")
endif()

FetchContent_Declare(
    battery-embed
    GIT_REPOSITORY https://github.com/batterycenter/embed.git
    GIT_TAG v1.2.19
)
FetchContent_MakeAvailable(battery-embed)


option(USE_FETCHCONTENT "fetching libs" FALSE)
if(USE_FETCHCONTENT)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
    )
    FetchContent_MakeAvailable(SFML)

    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
    )
    FetchContent_MakeAvailable(nlohmann_json)
else()
    find_package(SFML REQUIRED COMPONENTS Graphics Window System)
    find_package(nlohmann_json CONFIG REQUIRED)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_executable(svetlichki
    src/main.cpp
    src/winhandle.cpp
    src/Entity.cpp
    src/res.cpp
)
b_embed(svetlichki res/icon.png)

target_link_libraries(svetlichki PRIVATE SFML::Graphics SFML::Window SFML::System)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(svetlichki PRIVATE -mwindows -static -DB_PRODUCTION_MODE=ON)
    set_target_properties(svetlichki PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )
    add_custom_command(TARGET svetlichki POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:svetlichki>
            $<TARGET_FILE_DIR:svetlichki>
        COMMAND_EXPAND_LISTS
    )
endif()