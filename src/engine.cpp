#include "engine.h"

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
    this->window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT, window_name.c_str(), NULL, NULL
    );
    if (!this->window) {
        ERROR("Failed to create glfw window");
    }
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERROR("Failed to initialize GLAD");
    }
    glViewport(
        0,            // x
        0,            // y
        WINDOW_WIDTH, // width
        WINDOW_HEIGHT // height
    );
    glfwSetFramebufferSizeCallback(
        this->window,
        [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }
    );

    glfwSetKeyCallback(this->window, Engine::glfw_key_callback);
    glfwSetCursorPosCallback(this->window, Engine::glfw_cursor_pos_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // by default, disable mouse & keyboard inputs
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
    // disable input capture
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

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

    this->demo_init();
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
}

void Engine::render_loop() {
    INFO("Render loop started");
    while (!glfwWindowShouldClose(window)) {
        { // render loop prologue
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwPollEvents();
            { // imgui prologue
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
            }
        }
        this->delta_time.tick();
        this->camera->draw_debug_window();
        this->input->tick(this->delta_time.get());
        this->demo_tick();

        { // render loop epilogue

            { // imgui epilogue
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }
            glfwSwapBuffers(window);
        }
    }
    INFO("Render loop ended");
}

void Engine::demo_init() {

    // clang-format off

        float vertices[] = {
            // positions         // colors
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
             0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    // top 
            
             // the other triangle is slightly behind the main triangle(higher z value)
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   // bottom left
             0.0f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f    // top 
        };
    // clang-format on

    // generate one vertex array object
    glGenVertexArrays(1, &demo_data.vao);
    SPDLOG_INFO("Vao created: {}", demo_data.vao);
    // bind vertex array
    glBindVertexArray(demo_data.vao);
    // after binding, subsequent calls operate on `vao`
    { // vertex attr at location 0 and 1
        // buffer
        {
            unsigned int vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            // copy the vertices data to buffer
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(vertices), // num of bytes to copy over
                vertices,         // ptr to host memory
                GL_STATIC_DRAW    // update once, draw many times
            );
        }
        // set up vertex attribute arrs
        { // location = 0
            glVertexAttribPointer(
                0,        // location
                3,        // size of one vertex : how many elems in one vertex
                          // attribute
                GL_FLOAT, // data type of a single elem
                GL_FALSE, // only needed for int data type to normalize to
                          // [0, 1]
                6 * sizeof(float), // stride between vertices, 6 since we
                                   // have 3 floats to store rgb info
                (void*)0           // offset
            );
            glVertexAttribPointer(
                1,        // location
                3,        // size of one vertex : how many elems in one vertex
                          // attribute
                GL_FLOAT, // data type of a single elem
                GL_FALSE, // only needed for int data type to normalize to
                          // [0, 1]
                6 * sizeof(float),         // stride between vertices
                (void*)(3 * sizeof(float)) // offset
            );
            glEnableVertexAttribArray(0); // enable pointer at location 0
            glEnableVertexAttribArray(1); // enable pointer at location 1
        }
    }

    // shader setup
    // create & compile shader program
    this->demo_data.shaders = new SimpleShaderProgram(
        "../shaders/hello_triangle.vert", "../shaders/hello_triangle.frag"
    );
    bool success = this->demo_data.shaders->build();
    if (!success) {
        ERROR("Shader program building failed");
    }
}

void Engine::demo_tick() {
    this->demo_data.shaders->use();

    { // transform the triangle
        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(
            model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f)
        );
        // view matrix
        glm::mat4 view = camera->get_view_matrix();
        // proj matrix
        glm::mat4 proj = glm::perspective(
            glm::radians(FOV),
            (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT,
            0.1f,
            100.0f
        );

        // set transform matrices
        demo_data.shaders->set_uniform_mat4("u_model", model);
        demo_data.shaders->set_uniform_mat4("u_view", view);
        demo_data.shaders->set_uniform_mat4("u_proj", proj);
    }

    // bind vao
    glBindVertexArray(this->demo_data.vao);
    // draw triangles. currently we have 3 triangles
    glDrawArrays(
        GL_TRIANGLES, // mode
        0,            // first
        6             // count
    );
}

void Engine::toggle_imgui_control(bool on) {
    ImGuiIO& io = ImGui::GetIO();
    if (on) {
        io.ConfigFlags &= !ImGuiConfigFlags_NoMouse;
        io.ConfigFlags &= !ImGuiConfigFlags_NoKeyboard;
    } else {
        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
    }
}
