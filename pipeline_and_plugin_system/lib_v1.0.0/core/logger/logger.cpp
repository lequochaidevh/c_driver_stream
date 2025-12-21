#include "logger.hpp"

namespace ViPlugsEngine {
Shared<spdlog::logger> Logger::sCoreLogger   = nullptr;
Shared<spdlog::logger> Logger::sClientLogger = nullptr;

void Logger::init() {
    // spdlog::set_pattern("%^[%H:%M:%S] [%!:%#] [%n] [Thread:%t] %v%$");
    spdlog::set_pattern(
        "[%Y-%m-%d %H:%M:%S.%e] "  // HH:MM:SS.mmm + custom number
        "[%t]"                     // thread id
        " %^%l "                   // level (color)
        "[%s:%#] (%!)%$ "          // file:line:function
        "[%n] "                    // logger name
        "%v");                     // message
    sCoreLogger = spdlog::stdout_color_mt("ViPlugsEngine");
    sCoreLogger->set_level(spdlog::level::trace);
}

}  // namespace ViPlugsEngine