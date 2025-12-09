cmake_minimum_required(VERSION 3.16)

project(NTU_Rush LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/hitbox.json
               ${CMAKE_CURRENT_BINARY_DIR}/data/hitbox.json
               COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/roads.json
               ${CMAKE_CURRENT_BINARY_DIR}/data/roads.json
               COPYONLY)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Character.cpp
    src/Obstacle.cpp
    src/PowerUp.cpp
    src/Map.cpp
    src/Object.cpp
    src/Utils.cpp
)

add_executable(ntu_rush ${SOURCES})

target_include_directories(ntu_rush PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(ntu_rush PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    nlohmann_json::nlohmann_json
)
