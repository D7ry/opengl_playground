#include "engine.h"

#include "app/app.h"
#include "app/apps/coordinate_system.h"

int main() {
    Logging::init();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    Engine* engine = Engine::get_singleton();
    
    // add apps
    engine->register_app(std::make_unique<ImGuiApp>());
    engine->register_app(std::make_unique<MeshLoadingApp>());
    engine->register_app(std::make_unique<CoordinateSystemApp>());

    // initialize engine
    engine->init();

    // enter render loop
    engine->render_loop();
    
    glfwTerminate();
    return 0;
}
