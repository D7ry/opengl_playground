#include "ecs/system.h"
#include "mesh.h"
#include "tiny_engine.h"
#include "transform.h"

void TinyEngineApp::init(InitData& init_data) {
    this->system_manager = new SystemManager();
    PhongRenderSystem* render_system = new PhongRenderSystem();
    // push a node into render system
    PhongModel* model
        = new PhongModel("../assets/backpack.obj", init_data.texture_manager);
    Transform* transform = new Transform(Transform::identity());
    render_system->add_node(PhongRenderSystem::Node{model, transform});
    system_manager->add_system(render_system);
}

void TinyEngineApp::tick(TickData& tick_data) {
    this->system_manager->tick(&tick_data);
}
