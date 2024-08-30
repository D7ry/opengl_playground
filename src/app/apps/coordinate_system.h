#include "app/app.h"
#include "shader.h"

// render xyz coordinates
class CoordinateSystemApp : public App
{
  public:
    virtual void init(InitData& init_data) override;

    virtual void tick(TickData& tick_data) override;

    virtual ~CoordinateSystemApp() override;

  private:
    GLuint vao = 0;
    GLuint vbo = 0;
    std::unique_ptr<SimpleShaderProgram> shader;
};
