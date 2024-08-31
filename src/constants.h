#pragma once

// constant values

const glm::vec3 WORLD_UP_VECTOR = {0.f, 1.f, 0.f};
const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;
const size_t VIEWPORT_WIDTH = WINDOW_WIDTH;
const size_t VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const int WINDOW_CAPTURE_CURSOR_TOGGLE_KEY = GLFW_KEY_TAB;

const float FOV = 75.f;

const int CAMERA_SPEED = 3;

// model matrix uniform symbol
inline const char* U_MODEL = "u_model";
// view matrix uniform symbol
inline const char* U_VIEW = "u_view";
// projection matrix uniform symbol
inline const char* U_PROJ = "u_proj";

inline const char* WINDOW_TITLE = "New Engine";
