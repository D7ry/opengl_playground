#pragma once

#include "stdio.h"

// file io
#include <fstream>
#include <iostream>
#include <sstream>

#include "shader.h"

namespace Lab
{
namespace HelloTriangle
{
// init hellotriangle, set up buffers and send data to gpu
void init();

void tick();
} // namespace HelloTriangle

} // namespace Lab
