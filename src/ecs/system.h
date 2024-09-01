#pragma once

class Entity;
class TickData;

class ISystem
{
  public:
    ISystem() {}

    // iterate through all its nodes, and perform update logic on the nodes'
    // data
    virtual void tick(const TickData* tick_data) = 0;

    virtual ~ISystem() {}

    void add_entity(Entity* entity) { entities.push_back(entity); }

  protected:
    std::vector<Entity*> entities;
};

class PhongModel;
class Transform;
class ShaderProgram;

class PhongRenderSystem : public ISystem
{
  public:
    PhongRenderSystem();

    virtual void tick(const TickData* tick_data) override;

  private:
    ShaderProgram* shader;
};

class IDebugSystem : public ISystem
{
};

// view all entities in a side panel, similar to unity's object viewer
class EntityViewerGUISystem : public IDebugSystem
{
  public:
    EntityViewerGUISystem() {}

    virtual void tick(const TickData* tick_data) override;
};

class SystemManager
{
  public:
    void tick(TickData* tick_data) {
        for (ISystem* system : systems) {
            system->tick(tick_data);
        }
    }

    void add_system(ISystem* system) { this->systems.push_back(system); }

  private:
    std::vector<ISystem*> systems;
};
