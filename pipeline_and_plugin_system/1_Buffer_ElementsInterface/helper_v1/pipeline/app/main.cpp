#include "../core/pipeline_builder.hpp"
#include "../core/plugin_registry.hpp"
#include <iostream>

int main() {
    PluginRegistry registry;
    registry.scan("../plugins");

    Pipeline pipeline = PipelineBuilder::build_from_yaml("../config/metadata_pipeline.yaml", registry);

    if (!pipeline.init()) {
        std::cerr << "init failed\n";
        return -1;
    }

    if (!pipeline.start()) {
        std::cerr << "start failed\n";
        return -1;
    }

    constexpr int N   = 10000;
    BufferPtr     buf = std::make_shared<Buffer>(4096);

    for (int i = 0; i < N; i++) {
        if (!pipeline.run_once(buf)) {
            std::cerr << "run_once failed at " << i << std::endl;
            break;
        }

        if (i % 1000 == 0) std::cout << "Processed " << i << " buffers\n";
    }

    pipeline.stop();
    pipeline.shutdown();

    std::cout << "DONE\n";

    for (int i = 0; i < 100; i++) {
        Pipeline p = PipelineBuilder::build_from_yaml("../config/metadata_pipeline.yaml", registry);

        p.init();
        p.start();

        BufferPtr buf = std::make_shared<Buffer>(256);
        for (int j = 0; j < 100; j++) p.run_once(buf);

        p.stop();
        p.shutdown();
    }

    std::cout << "DONE 2 \n";
}