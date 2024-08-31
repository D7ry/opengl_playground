#include "app/app.h"
class PhongModel;

class PhongShadingApp : public App
{
  public:
    virtual void init(InitData& init_data) override;

    virtual void tick(TickData& tick_data) override;

  private:
    std::unique_ptr<PhongModel> model;
    std::unique_ptr<ShaderProgram> shader;
};
