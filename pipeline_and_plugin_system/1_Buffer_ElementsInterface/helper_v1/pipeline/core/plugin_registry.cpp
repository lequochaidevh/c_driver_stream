

// =====================================================
// plugin_registry.cpp
// =====================================================
// Không new Element trong registry

// Registry chỉ tạo khi app yêu cầu

#include "plugin_registry.hpp"
#include <filesystem>
#include <dlfcn.h>
#include <iostream>

namespace fs = std::filesystem;

bool PluginRegistry::scan(const std::string& directory) {
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

        std::cout << "Loaded plugin: " << entry.path() << std::endl;
    }
    return true;
}

std::unique_ptr<Element> PluginRegistry::create(const std::string& type) {
    auto it = plugins.find(type);
    if (it == plugins.end()) return nullptr;

    return std::unique_ptr<Element>(it->second.create_fn());
}

PluginRegistry::~PluginRegistry() {
    for (auto& [_, p] : plugins) dlclose(p.handle);
}
