#pragma once

#include "spdlog/spdlog.h"

#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define LOG_INFO(...)  ::Log::GetLogger()->info(fmt::format("INFO : {}", fmt::format(__VA_ARGS__)))
#define LOG_WARN(...)  ::Log::GetLogger()->warn(fmt::format("WARN : {}", fmt::format(__VA_ARGS__)))
#define LOG_ERROR(...) ::Log::GetLogger()->error(fmt::format("ERROR : {}", fmt::format(__VA_ARGS__)))

class Log
{
public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
private:
    static inline std::shared_ptr<spdlog::logger> s_Logger;
};
