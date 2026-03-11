#pragma once

#include "spdlog/spdlog.h"

#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define LOG_INFO(...)  ::Log::GetLogger()->info("INFO : {}", __VA_ARGS__)
#define LOG_WARN(...)  ::Log::GetLogger()->warn("WARN : {}", __VA_ARGS__)
#define LOG_ERROR(...) ::Log::GetLogger()->error("ERROR : {}", __VA_ARGS__)

class Log
{
public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
private:
    static inline std::shared_ptr<spdlog::logger> s_Logger;
};
