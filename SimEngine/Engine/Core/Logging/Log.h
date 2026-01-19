#pragma once

#include "spdlog/spdlog.h"

#define LOG_INFO(...)  ::Log::GetLogger()->info("INFO : {}", __VA_ARGS__)
#define LOG_WARN(...)  ::Log::GetLogger()->warn("WARN : {}", __VA_ARGS__)
#define LOG_ERROR(...) ::Log::GetLogger()->error("ERROR : {}", __VA_ARGS__)
#define LOG_TRACE(...) ::Log::GetLogger()->trace("TRACE : {}", __VA_ARGS__)
#define LOG_FATAL(...) ::Log::GetLogger()->fatal("FATAL : {}", __VA_ARGS__)

class Log
{
public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
private:
    static inline std::shared_ptr<spdlog::logger> s_Logger;
};
