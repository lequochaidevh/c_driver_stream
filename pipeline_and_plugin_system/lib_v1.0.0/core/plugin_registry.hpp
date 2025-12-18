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
    bool                     scan(const std::string& directory);
    std::unique_ptr<Element> create(const std::string& type);
    ~PluginRegistry();

 private:
    struct PluginInfo {
        void* handle            = nullptr;
        Element* (*create_fn)() = nullptr;
    };

    std::unordered_map<std::string, PluginInfo> plugins;
};
