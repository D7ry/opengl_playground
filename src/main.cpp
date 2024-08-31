#include "engine.h"

#include "app/app.h"
#include "app/apps/coordinate_system.h"
#include "app/apps/imgui_resource.h"

int main() {
    Logging::init();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    Engine* engine = Engine::get_singleton();
    
    // add apps
    engine->register_app(std::make_unique<ImguiResourceManagerApp>()); // imgui resource manager
    engine->register_app(std::make_unique<SimpleTriangleApp>()); // just 2 triangles
    engine->register_app(std::make_unique<MeshLoadingApp>()); // test mesh loading
    engine->register_app(std::make_unique<CoordinateSystemApp>()); // show x-y-z coordinate systems

    // initialize engine
    engine->init();

    // enter render loop
    engine->render_loop();
    
    glfwTerminate();
    return 0;
}
