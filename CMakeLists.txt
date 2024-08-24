cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OpenGL_playground)
# ---------- dependencies ---------- #
# OpenGL
find_package(OpenGL REQUIRED)
# glfw
find_package(glfw3 REQUIRED)
# spdlog
find_package(spdlog REQUIRED)
# glad
set(GLAD_SRC extern/glad/src/glad.c)
set(GLAD_INCLUDE extern/glad/include)

# ---------- Project Files ---------- #
add_executable(${PROJECT_NAME} 
    ${GLAD_SRC}
    src/opengl_playground_main.cpp
    src/input.cpp
    src/shader.cpp
    src/lab.cpp
)

# ---------- exeternal linkage ----------- #
target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARIES}
    spdlog::spdlog 
    glfw
)

# ---------- includes ---------- #
target_include_directories(${PROJECT_NAME} 
    PRIVATE ${GLAD_INCLUDE}
    PRIVATE ${OPENGL_INCLUDE_DIR}
    PRIVATE ${GLFW_INCLUDE_DIRS}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_precompile_headers(${PROJECT_NAME} 
    PRIVATE src/pch.h
)
