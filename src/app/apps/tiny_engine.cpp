#include "ecs/system.h"
#include "ecs/entity.h"
#include "mesh.h"
#include "tiny_engine.h"
#include "transform.h"

void TinyEngineApp::init(InitData& init_data) {
    this->system_manager = new SystemManager();
    PhongRenderSystem* render_system = new PhongRenderSystem();
    EntityViewerGUISystem* ev_system = new EntityViewerGUISystem();

    Entity* backpack = new Entity("backpack");
    backpack->add_component<PhongModel>("../assets/backpack.obj", init_data.texture_manager);
    backpack->add_component<Transform>();

    render_system->add_entity(backpack);
    ev_system->add_entity(backpack);

    system_manager->add_system(render_system);
    system_manager->add_system(ev_system);
}

void TinyEngineApp::tick(TickData& tick_data) {
    this->system_manager->tick(&tick_data);
}
