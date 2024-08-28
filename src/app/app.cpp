#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "app.h"

void SimpleTriangleApp::init(InitData& init_data) {
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
    glGenVertexArrays(1, &vao);
    INFO("Vao created: {}", vao);
    // bind vertex array
    glBindVertexArray(vao);
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
    this->shaders = new SimpleShaderProgram(
        "../shaders/hello_triangle.vert", "../shaders/hello_triangle.frag"
    );
    bool success = this->shaders->build();
    if (!success) {
        CRIT("Shader program building failed");
    }
}

void SimpleTriangleApp::tick(TickData& tick_data) {

    this->shaders->use();

    { // transform the triangle
        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(
            model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f)
        );
        // view matrix
        glm::mat4 view = tick_data.camera->get_view_matrix();
        // proj matrix
        glm::mat4 proj = glm::perspective(
            glm::radians(FOV),
            (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT,
            0.1f,
            100.0f
        );

        // set transform matrices
        shaders->set_uniform_mat4("u_model", model);
        shaders->set_uniform_mat4("u_view", view);
        shaders->set_uniform_mat4("u_proj", proj);
    }

    // bind vao
    glBindVertexArray(this->vao);
    // draw triangles. currently we have 3 triangles
    glDrawArrays(
        GL_TRIANGLES, // mode
        0,            // first
        6             // count
    );
}

void MeshLoadingApp::init(InitData& init_data) {
    this->shader = std::make_unique<SimpleShaderProgram>(
        "../shaders/phong_mesh.vert", "../shaders/phong_mesh.frag"
    );
    this->shader->build();
    this->model = std::make_unique<PhongModel>("../assets/backpack.obj");
}

void MeshLoadingApp::tick(TickData& tick_data) {

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f));
    // view matrix
    glm::mat4 view = tick_data.camera->get_view_matrix();
    // proj matrix
    glm::mat4 proj = glm::perspective(
        glm::radians(FOV),
        (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT,
        0.1f,
        100.0f
    );
    this->model->draw(model, view, proj, *this->shader);
}

void ImGuiApp::init(InitData& init_data) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // by default, disable mouse & keyboard inputs
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
    // disable input capture
    ImGui_ImplGlfw_InitForOpenGL(init_data.window, true);
    ImGui_ImplOpenGL3_Init();
}

void ImGuiApp::tickpre() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiApp::tickpost() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiApp::tick(TickData& tick_data) {
    ImGui::Begin("Hello Imgui");
    ImGui::End();
}


