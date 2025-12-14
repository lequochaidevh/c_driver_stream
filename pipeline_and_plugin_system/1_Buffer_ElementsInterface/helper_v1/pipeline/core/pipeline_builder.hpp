#pragma once

#include "pipeline.hpp"
#include "plugin_registry.hpp"
#include <string>

class PipelineBuilder {
 public:
    static std::unique_ptr<Pipeline> build_from_yaml(const std::string& path, PluginRegistry& registry);
};