cmake_minimum_required(VERSION 3.10)
project(svetlichki LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(FETCHCONTENT_UPDATES_DISCONNECTED OFF)
include(FetchContent)

option(USE_FETCHCONTENT "use fetching for json and sfml" FALSE)
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

FetchContent_Declare(
    battery-embed
    GIT_REPOSITORY https://github.com/batterycenter/embed.git
    GIT_TAG v1.2.19
)
FetchContent_MakeAvailable(battery-embed)

FetchContent_Declare(
    SFTransparent
    GIT_REPOSITORY https://github.com/rkc137/SFTransparent.git
    GIT_TAG 5f8beee1dcbd8bf7e4e0c1eda6d8220ed61b2c9b
)
FetchContent_MakeAvailable(SFTransparent)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_executable(svetlichki
    src/main.cpp
    src/Entity.cpp
    src/res.cpp

    # windows resources
    res/resource.rc
)

b_embed(${PROJECT_NAME} res/config.json)

target_compile_options(svetlichki PRIVATE -Wall -Wextra -Wno-missing-field-initializers)
target_link_libraries(svetlichki PRIVATE SFML::Graphics SFML::Window SFML::System SFTransparent)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O2)
    target_link_options(svetlichki PRIVATE -mwindows -DB_PRODUCTION_MODE=ON)
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