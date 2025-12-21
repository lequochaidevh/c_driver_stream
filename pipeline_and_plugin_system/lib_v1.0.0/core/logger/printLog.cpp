#include "printLog.hpp"

#include "logger.hpp"

namespace ViPlugsEngine {

#define LOG_WRAPPER(_LEVEL_ARG_)                                                                      \
    do {                                                                                              \
        va_list args;                                                                                 \
        va_start(args, fmt);                                                                          \
        char buffer[1024];                                                                            \
        vsnprintf(buffer, sizeof(buffer), fmt, args);                                                 \
        va_end(args);                                                                                 \
                                                                                                      \
        auto logger = Logger::getClientLogger();                                                      \
        if (logger) {                                                                                 \
            logger->log(spdlog::source_loc{ctx.file, ctx.line, ctx.func}, _LEVEL_ARG_, "{}", buffer); \
        }                                                                                             \
    } while (0)

PrintLog::PrintLog(const std::string& blockName, int set_level) {
    auto existing = spdlog::get(blockName);
    if (existing) {
        Logger::sClientLogger = existing;  // Have to assign directly to static member
    } else {
        Logger::sClientLogger = spdlog::stdout_color_mt(blockName);
        Logger::sClientLogger->set_level(spdlog::level::level_enum(set_level));
    }
}

void PrintLog::trace(const LogContext& ctx, const char* fmt, ...) { LOG_WRAPPER(spdlog::level::trace); }
void PrintLog::debug(const LogContext& ctx, const char* fmt, ...) { LOG_WRAPPER(spdlog::level::debug); }
void PrintLog::info(const LogContext& ctx, const char* fmt, ...) { LOG_WRAPPER(spdlog::level::info); }
void PrintLog::warn(const LogContext& ctx, const char* fmt, ...) { LOG_WRAPPER(spdlog::level::warn); }
void PrintLog::error(const LogContext& ctx, const char* fmt, ...) { LOG_WRAPPER(spdlog::level::err); }
void PrintLog::critical(const LogContext& ctx, const char* fmt, ...) { LOG_WRAPPER(spdlog::level::critical); }

}  // namespace ViPlugsEngine
