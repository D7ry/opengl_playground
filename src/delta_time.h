#pragma once
#include <chrono>

class DeltaTimer
{
  public:
    DeltaTimer();
    void tick();
    float get();

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _prev;
    float _delta = 0; // delta time in seconds
    bool _started = false;
};
