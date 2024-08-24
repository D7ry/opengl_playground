#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @brief Manages GLFW input, dispatches key hold callbacks
 *
 */
class InputManager
{
  public:
    static InputManager* get_singleton() {
        static InputManager singleton;
        return &singleton;
    }

    enum KeyCallbackCondition
    {
        PRESS,
        HOLD,
        RELEASE
    };

    /**
     * @brief key input function that should be bound to the glfw callback
     * function.
     */
    void on_key_input(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods
    );

    void clear_keys();

    /**
     * @brief Update function to be called once every tick. To dispatch the hold
     * callback function.
     */
    void tick(float deltaTime);

    /**
     * @brief Register a single key callback
     *
     * @param key the key to register
     * @param callbackCondition the condition of the input for the callback to
     * dispatch
     * @param callback the callback function.
     */
    void register_key_callback(
        int key,
        KeyCallbackCondition callbackCondition,
        std::function<void()> callback
    );

  private:
    /**
     * @brief keys that are currently being pressed down
     */
    std::unordered_set<int> _heldKeys;

    /**
     * @brief Callback functions mapped to the keys, and the conditions to
     * trigger them.
     *
     */
    std::unordered_map<int, std::vector<std::function<void()>>> callback_press
        = {};
    std::unordered_map<int, std::vector<std::function<void()>>> callback_hold
        = {};
    std::unordered_map<int, std::vector<std::function<void()>>> callback_release
        = {};

    struct ComboKeyMap
    {
        std::vector<int> _keys;
        std::function<void()> func;
    };

    std::vector<ComboKeyMap> combo_callbacks;
};
