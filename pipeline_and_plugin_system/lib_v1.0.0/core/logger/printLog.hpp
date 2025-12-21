#pragma once
#include "../unit/pre_compile_header_file.hpp"

namespace ViPlugsEngine {

struct LogContext {
    const char* file;
    int         line;
    const char* func;
};

class PrintLog {
 public:
    PrintLog(const std::string& blockName = "Client", int set_level = 0);

    static void trace(const LogContext& ctx, const char* log, ...);

    static void debug(const LogContext& ctx, const char* log, ...);

    static void info(const LogContext& ctx, const char* log, ...);

    static void warn(const LogContext& ctx, const char* log, ...);

    static void error(const LogContext& ctx, const char* log, ...);

    static void critical(const LogContext& ctx, const char* log, ...);
};

#define _CLIENT_DEBUG 1

static PrintLog printLog("Application", 1);  // 0 = trace

#define LOG_CTX \
    LogContext { __FILE__, __LINE__, __func__ }

#if _CLIENT_DEBUG

#define LOG_TRACE(...)    printLog.trace(LOG_CTX, __VA_ARGS__)
#define LOG_DEBUG(...)    printLog.debug(LOG_CTX, __VA_ARGS__)
#define LOG_INFO(...)     printLog.info(LOG_CTX, __VA_ARGS__)
#define LOG_WARN(...)     printLog.warn(LOG_CTX, __VA_ARGS__)
#define LOG_ERROR(...)    printLog.error(LOG_CTX, __VA_ARGS__)
#define LOG_CRITICAL(...) printLog.critical(LOG_CTX, __VA_ARGS__)
#else

#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)
#endif
}  // namespace ViPlugsEngine