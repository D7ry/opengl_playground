#include "engine.h"
#include "mesh.h"

void Engine::glfw_cursor_pos_callback(
    GLFWwindow* window,
    double xpos,
    double ypos
) {
    static bool first_mouse_input = true;
    static double prev_x = 0;
    static double prev_y = 0;
    if (first_mouse_input) {
        first_mouse_input = false;
        prev_x = xpos;
        prev_y = ypos;
        return;
    }
    float offset_x = xpos - prev_x;
    float offset_y = ypos - prev_y;
    prev_x = xpos;
    prev_y = ypos;

    float sensitivity = 0.1f;
    offset_x *= sensitivity;
    offset_y *= sensitivity;

    Engine::get_singleton()->camera->mod_yaw(offset_x);
    Engine::get_singleton()->camera->mod_pitch(-offset_y);
}


void Engine::glfw_fb_resize_callback(
    GLFWwindow* window, 
    int width, 
    int height
) {
    DEBUG("window resized to : {} {}", width, height);
    glViewport(0, 0, width, height);
}

void Engine::glfw_key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
) {
    Engine::get_singleton()->input->on_key_input(
        window, key, scancode, action, mods
    );
}

Engine::Engine(const std::string& window_name) {
    this->apps.push_back(std::make_unique<ImGuiApp>());
    this->apps.push_back(std::make_unique<MeshLoadingApp>());

    this->window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT, window_name.c_str(), NULL, NULL
    );
    if (!this->window) {
        CRIT("Failed to create glfw window");
    }
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        CRIT("Failed to initialize GLAD");
    }
    glViewport(
        0,            // x
        0,            // y
        WINDOW_WIDTH, // width
        WINDOW_HEIGHT // height
    );
    glfwSetFramebufferSizeCallback(this->window, Engine::glfw_fb_resize_callback);
    glfwSetKeyCallback(this->window, Engine::glfw_key_callback);
    glfwSetCursorPosCallback(this->window, Engine::glfw_cursor_pos_callback);


    this->input = std::make_unique<InputManager>();
    this->camera = std::make_unique<Camera>(
        glm::vec3{
            0.f, // x
            0.f, // y
            0.f  // z
        },
        -90, // yaw
        0,   // pitch
        0    // roll
    );
    this->bind_default_inputs();

    glEnable(GL_DEPTH_TEST);

    App::InitData init_data{window, std::addressof(this->texture_manager)};
    for (std::unique_ptr<App>& app : this->apps) {
        app->init(init_data);
    }
};

Engine* Engine::get_singleton() {
    static Engine singleton("New Engine");
    return std::addressof(singleton);
}

void Engine::bind_default_inputs() {
    input->register_key_callback(
        GLFW_KEY_ESCAPE,
        InputManager::KeyCallbackCondition::PRESS,
        [this]() { glfwSetWindowShouldClose(this->window, true); }
    );
    input->register_key_callback(
        GLFW_KEY_W,
        InputManager::KeyCallbackCondition::HOLD,
        [this]() {
            camera->mod_position(0, 0, -delta_time.get() * CAMERA_SPEED);
        }
    );
    input->register_key_callback(
        GLFW_KEY_S,
        InputManager::KeyCallbackCondition::HOLD,
        [this]() {
            camera->mod_position(0, 0, delta_time.get() * CAMERA_SPEED);
        }
    );
    input->register_key_callback(
        GLFW_KEY_A,
        InputManager::KeyCallbackCondition::HOLD,
        [this]() {
            camera->mod_position(-delta_time.get() * CAMERA_SPEED, 0, 0);
        }
    );
    input->register_key_callback(
        GLFW_KEY_D,
        InputManager::KeyCallbackCondition::HOLD,
        [this]() {
            camera->mod_position(delta_time.get() * CAMERA_SPEED, 0, 0);
        }
    );
    input->register_key_callback(
        GLFW_KEY_SPACE,
        InputManager::KeyCallbackCondition::HOLD,
        [this]() {
            camera->mod_position(0, delta_time.get() * CAMERA_SPEED, 0);
        }
    );
    input->register_key_callback(
        GLFW_KEY_LEFT_CONTROL,
        InputManager::KeyCallbackCondition::HOLD,
        [this]() {
            camera->mod_position(0, -delta_time.get() * CAMERA_SPEED, 0);
        }
    );
    input->register_key_callback(
        GLFW_KEY_TAB,
        InputManager::KeyCallbackCondition::PRESS,
        [this]() { toggle_imgui_control(); }
    );
    input->register_key_callback(
        GLFW_KEY_F1,
        InputManager::KeyCallbackCondition::PRESS,
        [this]() { toggle_cursor_capture(); }
    );
}

void Engine::render_loop() {
    INFO("Render loop started");
    while (!glfwWindowShouldClose(window)) {
        { // render loop prologue
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glfwPollEvents();
            for (auto& app : this->apps) {
                app->tickpre();
            }
        }
        { // render loop

            this->delta_time.tick();
            this->draw_debug_window();
            this->input->tick(this->delta_time.get());
            App::TickData tick_data{this->camera.get()};
            for (auto& app : this->apps) {
                app->tick(tick_data);
            }
        }
        { // render loop epilogue
            for (auto& app : this->apps) {
                app->tickpost();
            }
            glfwSwapBuffers(window);
        }
    }
    INFO("Render loop ended");
}

void Engine::toggle_imgui_control() {
    this->imgui_control = !this->imgui_control;
    ImGuiIO& io = ImGui::GetIO();
    if (this->imgui_control) {
        DEBUG("Enabled imgui input");
        io.ConfigFlags &= !ImGuiConfigFlags_NoMouse;
        io.ConfigFlags &= !ImGuiConfigFlags_NoKeyboard;
    } else {
        DEBUG("Disabled imgui input");
        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
    }
}

void Engine::toggle_cursor_capture() {
    this->cursor_capture = !this->cursor_capture;
    if (this->cursor_capture) {
        DEBUG("capture cursor");
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        DEBUG("release cursor");
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
