cmake_minimum_required(VERSION 3.10)
project(svetlichki LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(FETCHCONTENT_UPDATES_DISCONNECTED OFF)
include(FetchContent)
if(DEFINED ENV{CMAKE_LIBS_DIR})
    set(FETCHCONTENT_BASE_DIR "$ENV{CMAKE_LIBS_DIR}")
endif()

find_package(SFML REQUIRED COMPONENTS Graphics Window System)
find_package(nlohmann_json CONFIG REQUIRED)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_executable(svetlichki
    src/main.cpp
    src/winhandle.cpp
)

target_link_libraries(svetlichki PRIVATE SFML::Graphics SFML::Window SFML::System)