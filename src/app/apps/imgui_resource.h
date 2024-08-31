#pragma once
#include "app/app.h"

// app to set up resources, prologue and epilogues for imgui drawing.
class ImguiResourceManagerApp : public App
{
    virtual void init(InitData& init_data) override;

    virtual void tickpre() override;

    virtual void tick(TickData& tick_data) override;

    virtual void tickpost() override;
};
