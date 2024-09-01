#pragma once

class TickData;

class ISystem
{
  public:
    ISystem() {}

    // iterate through all its nodes, and perform update logic on the nodes'
    // data
    virtual void tick(const TickData* tick_data) = 0;

    virtual ~ISystem() {}
};

class PhongModel;
class Transform;
class ShaderProgram;

class PhongRenderSystem : public ISystem
{
  public:
    PhongRenderSystem();

    struct Node
    { // node contains ptr to components
        PhongModel* model;
        Transform* transform;
    };

    virtual void tick(const TickData* tick_data) override;

    void add_node(Node node) { this->nodes.push_back(std::move(node)); }

  private:
    std::vector<Node> nodes;
    ShaderProgram* shader;
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
