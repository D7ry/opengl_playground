#include <functional>
#include <typeindex>

#include "mesh.h"
#include "shared.h"
#include "transform.h"

class Component
{
  public:
    virtual ~Component() = default;

    virtual void tick(const TickData& tick_data) {}
};

class RenderComponent : public Component
{

};

// TODO: don't do too much before implementing skeletal transforms
class Object
{
  public:
    void init(std::string name, Transform initial_transform) {
        this->name = std::move(name);
        this->transform = std::move(initial_transform);
    }

    template <typename T, typename... Args>
    T* add_component(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* rawPtr = component.get();
        components[std::type_index(typeid(T))] = std::move(component);
        return rawPtr;
    }

    // update object internal states
    void tick(const TickData& tick_data) {
        for (const auto& pair : this->components) {
            pair.second->tick(tick_data);
        }
    }

  private:
    Transform transform;
    std::string name;

    // an object can only hold one component of any subclass of components
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};
