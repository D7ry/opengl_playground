#pragma once

// Global includes and macro configurations

// turn off clang-format due to rearranging includes, glad needs to come before
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// clang-format on

// cpp stl
#include <vector>
#include <string>

// glm
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

// imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
