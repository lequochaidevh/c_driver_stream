// =====================================================
// plugin_registry.cpp
// =====================================================
// NOT new Element in registry
// Only Registry when app required

#include "plugin_registry.hpp"
#include "logger/logger.hpp"

namespace fs = std::filesystem;

namespace ViPlugsEngine {

bool PluginRegistry::scan(const std::string& directory) {
    Environment::init();
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() != ".so") continue;

        void* handle = dlopen(entry.path().c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << dlerror() << std::endl;
            continue;
        }

        Element* (*create_fn)() = (Element * (*)()) dlsym(handle, "create");
        if (!create_fn) {
            dlclose(handle);
            continue;
        }

        Element* tmp         = create_fn();
        plugins[tmp->name()] = {handle, create_fn};
        delete tmp;

        CORE_LOG_DEBUG("Loaded plugin: {}", entry.path().string());
    }
    return true;
}

std::unique_ptr<Element> PluginRegistry::create(const std::string& type) {
    auto it = plugins.find(type);
    if (it == plugins.end()) return nullptr;

    return std::unique_ptr<Element>(it->second.create_fn());  // Becareful if not use normal ptr
}

PluginRegistry::~PluginRegistry() {
    for (auto& [_, p] : plugins) dlclose(p.handle);
}

}  // namespace ViPlugsEngine