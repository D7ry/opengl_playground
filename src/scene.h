#pragma once
#include "light.h"
#include "object.h"

class Scene
{
  public:
    // draw all objects as well as auxiliary stuff
    void draw();

  private:
    std::vector<Light> lights;
    std::vector<Object> objects;

};
