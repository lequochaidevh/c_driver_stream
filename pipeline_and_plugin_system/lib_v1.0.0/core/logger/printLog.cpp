#include "printLog.hpp"

#include "logger.hpp"

namespace ViPlugsEngine {

#define LOG_WRAPPER(_LEVEL_ARG_)                                                                             \
    do {                                                                                                     \
        va_list args;                                                                                        \
        va_start(args, fmt);                                                                                 \
        char buffer[1024];                                                                                   \
        vsnprintf(buffer, sizeof(buffer), fmt, args);                                                        \
        va_end(args);                                                                                        \
                                                                                                             \
        auto logger = Logger::getClientLogger();                                                             \
        if (logger) {                                                                                        \
            logger->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, _LEVEL_ARG_, "{}", buffer); \
        }                                                                                                    \
    } while (0)

void PrintLog::debug(const char* fmt, ...) { LOG_WRAPPER(spdlog::level::debug); }
void PrintLog::info(const char* fmt, ...) { LOG_WRAPPER(spdlog::level::info); }
void PrintLog::warn(const char* fmt, ...) { LOG_WRAPPER(spdlog::level::warn); }
void PrintLog::error(const char* fmt, ...) { LOG_WRAPPER(spdlog::level::err); }
void PrintLog::critical(const char* fmt, ...) { LOG_WRAPPER(spdlog::level::critical); }

}  // namespace ViPlugsEngine
