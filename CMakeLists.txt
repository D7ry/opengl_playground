cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OpenGL_playground)
# ---------- dependencies ---------- #
set(GLAD_SRC extern/glad/src/glad.c)
set(GLAD_INCLUDE extern/glad/include)
set(STB_INCLUDE extern/stb)
set(STB_IMG_SRC extern/stb/stb_image_impl.cpp)
set(GLM_INCLUDE extern/glm)
set(GLFW_INCLUDE extern/glfw/include)
set(SPDLOG_INCLUDE extern/spdlog/include)

# submodules
add_subdirectory(extern/glfw)
add_subdirectory(extern/spdlog)

# ---------- Project Files ---------- #
add_executable(${PROJECT_NAME} 
    ${GLAD_SRC}
    ${STB_IMG_SRC}
    src/opengl_playground_main.cpp
    src/input.cpp
    src/camera.cpp
    src/shader.cpp
    src/lab.cpp
    src/delta_time.cpp
)

# ---------- exeternal linkage ----------- #
target_link_libraries(${PROJECT_NAME} 
    spdlog::spdlog 
    glfw
)

# ---------- includes ---------- #
target_include_directories(${PROJECT_NAME} 
    PRIVATE ${STB_INCLUDE}
    PRIVATE ${GLAD_INCLUDE}
    PRIVATE ${GLFW_INCLUDE}
    PRIVATE ${GLM_INCLUDE}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_precompile_headers(${PROJECT_NAME} 
    PRIVATE src/pch.h
)
