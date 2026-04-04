cmake_minimum_required(VERSION 4.1)
project(minecraft_clone)

set(CMAKE_CXX_STANDARD 20)

macro(add_glfw)
    add_subdirectory(vendors/glfw)
    target_link_libraries(minecraft_clone PRIVATE glfw)
endmacro()

macro(add_glad)
    target_sources(minecraft_clone PRIVATE vendors/glad/src/glad.c)
    target_include_directories(minecraft_clone PRIVATE vendors/glad/include)
endmacro()

macro(add_glm)
    target_include_directories(minecraft_clone PRIVATE vendors/glm)
endmacro()

add_executable(minecraft_clone main.cpp
        render.cpp
        render.h
        chunks.cpp
        chunks.h
        player.cpp
        player.h
        application.cpp
        application.h)

add_glad()
add_glfw()
add_glm()