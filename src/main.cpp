#include "engine.h"

#include "app/app.h"
#include "app/apps/coordinate_system.h"
#include "app/apps/imgui_resource.h"
#include "app/apps/phong_shading.h"
#include "app/apps/tiny_engine.h"

int main() {
    Logging::init();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    Engine* engine = Engine::get_singleton();
    
    // add apps
    // required app, don't touch
    engine->register_app(std::make_unique<ImguiResourceManagerApp>()); // imgui resource manager

    // user-space app
    //engine->register_app(std::make_unique<SimpleTriangleApp>()); // just 2 triangles
    engine->register_app(std::make_unique<CoordinateSystemApp>()); // show x-y-z coordinate systems
    //engine->register_app(std::make_unique<PhongShadingApp>()); // phong shading

    engine->register_app(std::make_unique<TinyEngineApp>());
    // initialize engine
    engine->init();

    // enter render loop
    engine->render_loop();
    
    glfwTerminate();
    return 0;
}
