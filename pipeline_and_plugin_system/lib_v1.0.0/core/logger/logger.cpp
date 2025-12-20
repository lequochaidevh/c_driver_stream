#include "logger.hpp"

namespace ViPlugsEngine {
Shared<spdlog::logger> Logger::sCoreLogger   = nullptr;
Shared<spdlog::logger> Logger::sClientLogger = nullptr;

// Pattern: [22:40:00] [ViPlugsEngine::Logger:Init:11] [ViPlugsEngine] [Thread:1000] Logger message
void Logger::init() {
    spdlog::set_pattern("%^[%H:%M:%S] [%!:%#] [%n] [Thread:%t] %v%$");

    sCoreLogger = spdlog::stdout_color_mt("ViPlugsEngine");
    sCoreLogger->set_level(spdlog::level::trace);

    sClientLogger = spdlog::stdout_color_mt("Client");
    sClientLogger->set_level(spdlog::level::trace);
}

}  // namespace ViPlugsEngine