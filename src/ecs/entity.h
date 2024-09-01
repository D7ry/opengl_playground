#pragma once
#include "component.h"
#include <typeindex>

class Component;

class Entity
{
  public:
    Entity(const std::string& name) { this->name = name; }

    template <typename T, typename... Args>
    void add_component(Args&&... args) {
        components.emplace(
            std::type_index(typeid(T)), new T(std::forward<Args>(args)...)
        );
    }

    template <typename T>
    T* get_component() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it == components.end()) {
            return nullptr;
        }
        return static_cast<T*>(it->second);
    }

    const char* get_name() { return this->name.c_str(); }

  private:
    std::unordered_map<std::type_index, Component*> components;
    std::string name;
};
