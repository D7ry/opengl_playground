#pragma once
#include "shared.h"

class Camera;
class TextureManager;

// app loaded by the engine
// each app contains
// `init()` method -- invoked post engine initialization
// `tick()` method -- invoked every single engine tick, where the draw calls
// should be placed
class App
{
  public:

    virtual void init(InitData& init_data) {
        CRIT("Default initializer not implemented");
    }

    virtual void tickpre(){};

    virtual void tick(TickData& tick_data) {
        CRIT("Default initializer not implemented");
    }

    virtual void tickpost(){};

    // draw imgui debug info. Assuming a parent window (ImGui::Begin() has been
    // invoked prior)
    virtual void draw_debug_info(){};

    virtual ~App() = default;
};

class ShaderProgram;
class SimpleShaderProgram;

class SimpleTriangleApp : public App
{
  public:
    virtual void init(InitData& init_data) override;

    virtual void tick(TickData& tick_data) override;

    virtual void draw_debug_info() override {
        ImGui::Separator();
        ImGui::Text("Hello from simple triangle app");
    }

  private:
    unsigned int vao;
    SimpleShaderProgram* shaders = nullptr;
};

