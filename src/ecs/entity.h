#pragma once
#include <typeindex>

class Component;

class Entity
{
  public:
    template <typename T, typename... Args>
    void add_component(Args&&... args) {
        components.emplace(std::type_index(typeid(T)), new T(std::forward<Args>(args)...));
    }

    template <typename T>
    T* get_component() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it == components.end()) {
            return nullptr;
        }
        return static_cast<T*>(it->second);
    }

  private:
    std::unordered_map<std::type_index, Component*> components;
};
