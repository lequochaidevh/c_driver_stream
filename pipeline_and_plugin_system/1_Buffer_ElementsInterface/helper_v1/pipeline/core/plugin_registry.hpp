// =====================================================
// plugin_registry.hpp
// Scan directory, load multiple plugins (.so)
// =====================================================
#pragma once

#include "pipeline_core.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class PluginRegistry {
 public:
    bool     scan(const std::string& directory);
    Element* create(const std::string& name);
    ~PluginRegistry();

 private:
    struct PluginInfo {
        void* handle            = nullptr;
        Element* (*create_fn)() = nullptr;
    };

    std::unordered_map<std::string, PluginInfo> plugins;
};
