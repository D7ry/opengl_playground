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
set(IMGUI_INCLUDE extern/imgui)
set(ASSIMP_INCLUDE extern/assimp/include)

set(IMGUI_SRC 
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
)

# submodules
add_subdirectory(extern/glfw)
add_subdirectory(extern/spdlog)
add_subdirectory(extern/assimp)


# ---------- Project Files ---------- #
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/input.cpp
    src/camera.cpp
    src/shader.cpp
    # src/lab.cpp
    src/delta_time.cpp
    src/engine.cpp
    src/mesh.cpp
    src/texture.cpp
    src/app/app.cpp
    src/app/apps/coordinate_system.cpp
)

# library sources
target_sources(${PROJECT_NAME} 
    PRIVATE ${GLAD_SRC}
    PRIVATE ${STB_IMG_SRC}
    PRIVATE ${IMGUI_SRC}
)

# ---------- exeternal linkage ----------- #
target_link_libraries(${PROJECT_NAME} 
    spdlog::spdlog 
    glfw
    assimp
)

# ---------- includes ---------- #
target_include_directories(${PROJECT_NAME} 
    PRIVATE ${STB_INCLUDE}
    PRIVATE ${GLAD_INCLUDE}
    PRIVATE ${GLFW_INCLUDE}
    PRIVATE ${GLM_INCLUDE}
    PRIVATE ${IMGUI_INCLUDE}
    PRIVATE ${ASSIMP_INCLUDE}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE src
)

target_precompile_headers(${PROJECT_NAME} 
    PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
)

# ---------- macros ----------#
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_definitions(NDEBUG)
endif()
