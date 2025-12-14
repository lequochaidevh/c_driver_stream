#include "../core/pipeline_builder.hpp"
#include "../core/plugin_registry.hpp"

int main() {
    PluginRegistry registry;
    registry.scan("../plugins");

    Pipeline pipeline = PipelineBuilder::build_from_yaml("../config/metadata_pipeline.yaml", registry);

    if (!pipeline.init()) return -1;
    if (!pipeline.start()) return -1;

    BufferPtr buf = std::make_shared<Buffer>(4096);

    for (int i = 0; i < 4; i++) {
        pipeline.run_once();
    }

    for (int i = 0; i < 5; i++) {
        pipeline.run_once(buf);
    }

    pipeline.shutdown();
}