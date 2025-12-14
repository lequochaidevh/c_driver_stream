// -----------------------------------------------------
// pipeline_builder.cpp
#include "pipeline_builder.hpp"
#include <yaml-cpp/yaml.h>

// Pipeline PipelineBuilder::build_from_yaml(const std::string& yaml_path, PluginRegistry& registry) {
//     Pipeline   pipeline;
//     YAML::Node cfg = YAML::LoadFile(yaml_path);

//     for (const auto& node : cfg["pipeline"]) {
//         std::string type = node["type"].as<std::string>();
//         Element*    e    = registry.create(type);
//         if (!e) {
//             throw std::runtime_error("Plugin not found: " + type);
//         }

//         pipeline.add(e);
//     }
//     return pipeline;
// }std::unique_ptr<Element> PluginRegistry::create(const std::string& type)

std::unique_ptr<Pipeline> PipelineBuilder::build_from_yaml(const std::string& path, PluginRegistry& registry) {
    auto pipeline = std::make_unique<Pipeline>();

    YAML::Node cfg = YAML::LoadFile(path);
    for (const auto& node : cfg["pipeline"]) {
        std::string type = node["type"].as<std::string>();
        pipeline->add(registry.create(type));
    }

    return pipeline;  // MOVE unique_ptr (OK)
}

// sudo apt install libyaml-cpp-dev