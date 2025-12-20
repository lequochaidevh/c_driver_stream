#pragma once
#include "../unit/pre_compile_header_file.hpp"

namespace ViPlugsEngine {

class PrintLog {
 public:
    static void debug(const char* log, ...);

    static void info(const char* log, ...);

    static void warn(const char* log, ...);

    static void error(const char* log, ...);

    static void critical(const char* log, ...);
};

static PrintLog printLog;

}  // namespace ViPlugsEngine