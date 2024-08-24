#include "input.h"

void InputManager::on_key_input(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
) {
    switch (action) {
    case GLFW_PRESS:
        _heldKeys.insert(key);
        for (auto& callback : callback_press[key]) {
            callback();
        }
        break;
    case GLFW_RELEASE:
        _heldKeys.erase(key);
        for (auto& callback : callback_release[key]) {
            callback();
        }
        break;
    default:
        break;
    }
}

void InputManager::clear_keys() { _heldKeys.clear(); }

void InputManager::tick(float deltaTime) {
    for (auto& key : _heldKeys) {
        for (auto& callback : callback_hold[key]) {
            callback();
        }
    }
}

void InputManager::register_key_callback(
    int key,
    KeyCallbackCondition callbackCondition,
    std::function<void()> callback
) {
    switch (callbackCondition) {
    case PRESS:
        if (callback_press.find(key) == callback_press.end()) {
            callback_press[key] = std::vector<std::function<void()>>();
        }
        callback_press[key].push_back(callback);
        break;
    case HOLD:
        if (callback_hold.find(key) == callback_hold.end()) {
            callback_hold[key] = std::vector<std::function<void()>>();
        }
        callback_hold[key].push_back(callback);
        break;
    case RELEASE:
        if (callback_release.find(key) == callback_release.end()) {
            callback_release[key] = std::vector<std::function<void()>>();
        }
        callback_release[key].push_back(callback);
        break;
    default:
        break;
    }
}
