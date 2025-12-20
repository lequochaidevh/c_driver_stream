// -----------------------------------------------------
#include "pipeline_builder.hpp"
#include <yaml-cpp/yaml.h>

namespace ViPlugsEngine {

std::unique_ptr<Pipeline> PipelineBuilder::build_from_yaml(const std::string& path, PluginRegistry& registry) {
    auto pipeline = std::make_unique<Pipeline>();

    YAML::Node cfg = YAML::LoadFile(path);
    for (const auto& node : cfg["pipeline"]) {
        std::string type = node["type"].as<std::string>();
        pipeline->add(registry.create(type));
    }

    return pipeline;  // MOVE unique_ptr (OK)
}

}  // namespace ViPlugsEngine

// sudo apt install libyaml-cpp-dev