#pragma once
// logging utilities

// enable debug logging
#ifndef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif

#include <spdlog/spdlog.h>

#define INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define CRIT(...) SPDLOG_CRITICAL(__VA_ARGS__); exit(1);

namespace Logging
{
    // initialize logging utilities, set up patterns
    inline void init() {
        spdlog::set_pattern("\033[1;37m[%^%l%$] [%s:%#] (%!) %v\033[0m");
#ifdef NDEBUG // release
        spdlog::set_level(spdlog::level::info);
#else
        spdlog::set_level(spdlog::level::debug);
#endif
    }
}
