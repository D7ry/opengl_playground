#pragma once
#include "light.h"
#include "object.h"

class Scene
{
  public:
    // draw all objects as well as auxiliary stuff
    // todo: deferred render objects
    void draw();

  //private: todo add it back
    std::vector<Light> lights;
    std::vector<std::unique_ptr<Object>> objects;

};

static Scene* get_experimental_scene() 
{
    static Scene* scene;
    if (scene) {
        return scene;
    }

    scene = new Scene();


    return scene;
};
