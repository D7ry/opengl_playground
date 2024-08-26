#include "engine.h"

int main() {
    Logging::init();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    Engine* engine = Engine::get_singleton();
    engine->render_loop();
    
    glfwTerminate();
    return 0;
}
