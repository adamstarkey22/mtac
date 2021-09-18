cmake_minimum_required(VERSION 3.2)

project(mtac LANGUAGES CXX)

add_subdirectory(ext/SFML)

# main executable
add_executable(mtac
    src/main.cc
    src/Tileset.cc
    src/Board.cc
    src/BoardRenderer.cc
)

target_include_directories(mtac PRIVATE ext/SFML/include)
target_link_libraries(mtac sfml-graphics)