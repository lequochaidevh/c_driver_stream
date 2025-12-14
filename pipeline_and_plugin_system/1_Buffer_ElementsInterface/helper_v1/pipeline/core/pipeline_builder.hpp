#pragma once

#include "pipeline.hpp"
#include "plugin_registry.hpp"
#include <string>

class PipelineBuilder {
 public:
    static Pipeline build_from_yaml(const std::string& yaml_path, PluginRegistry& registry);
};