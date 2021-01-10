cmake_minimum_required(VERSION 3.18.2)

set(TARGET musicPlayer)

project(musicPlayer LANGUAGES CXX)

project(${TARGET} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(3dparty)
add_subdirectory(app)
