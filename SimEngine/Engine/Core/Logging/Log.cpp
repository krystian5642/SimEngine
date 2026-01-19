#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    
    s_Logger = spdlog::stdout_color_mt("SimEngine");
    s_Logger->set_level(spdlog::level::trace);
}
