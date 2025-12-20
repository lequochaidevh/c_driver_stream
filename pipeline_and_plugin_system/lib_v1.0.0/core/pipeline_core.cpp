#include "pipeline_core.hpp"
#include "logger/logger.hpp"
#include "logger/printLog.hpp"
// g++ -fPIC -shared pipeline_core.cpp -o libpipeline_core.so

void ViPlugsEngine::Environment::init() {
    static std::once_flag once;
    std::call_once(once, [] {
        Logger::init();
        // Firstime::init();
    });
}
