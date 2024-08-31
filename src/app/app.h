#pragma once

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
    struct InitData
    {
        GLFWwindow* window;
        TextureManager* texture_manager;
    };

    struct TickData
    {
        Camera* camera;
        TextureManager* texture_manager;
        double dt; // delta time
        glm::mat4 proj; // projection matrix of main viewport
    };

    virtual void init(InitData& init_data) {
        CRIT("Default initializer not implemented");
    }

    virtual void tickpre(){};

    virtual void tick(TickData& tick_data) {
        CRIT("Default initializer not implemented");
    }

    virtual void tickpost(){};

    virtual ~App() = default;
};

// app to set up resources, prologue and epilogues for imgui drawing.
class ImGuiApp : public App
{

    virtual void init(InitData& init_data) override;

    virtual void tickpre() override;

    virtual void tick(TickData& tick_data) override;

    virtual void tickpost() override;
};

class ShaderProgram;
class SimpleShaderProgram;

class SimpleTriangleApp : public App
{
  public:
    virtual void init(InitData& init_data) override;

    virtual void tick(TickData& tick_data) override;

  private:
    unsigned int vao;
    SimpleShaderProgram* shaders = nullptr;
};

class PhongModel;

class MeshLoadingApp : public App
{
  public:
    virtual void init(InitData& init_data) override;

    virtual void tick(TickData& tick_data) override;

  private:
    std::unique_ptr<PhongModel> model;
    std::unique_ptr<ShaderProgram> shader;
};
