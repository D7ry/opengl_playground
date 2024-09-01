#pragma once
#include "app/app.h"

class SystemManager;

// prototype game engine
class TinyEngineApp : public App
{
  public:
    virtual void init(InitData& init_data) override;
    virtual void tick(TickData& tick_data) override;

  private:
    SystemManager* system_manager;
};
