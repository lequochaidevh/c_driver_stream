#include "../core/pipeline_builder.hpp"
#include "../core/plugin_registry.hpp"

// int main() {
//     PluginRegistry registry;
//     registry.scan("../plugins");

//     std::unique_ptr<Pipeline> pipeline = PipelineBuilder::build_from_yaml("../config/metadata_pipeline.yaml",
//     registry);

//     if (!pipeline->init()) {
//         std::cerr << "init failed\n";
//         return -1;
//     }

//     if (!pipeline->start()) {
//         std::cerr << "start failed\n";
//         return -1;
//     }

//     constexpr int N   = 10000;
//     BufferPtr     buf = std::make_shared<Buffer>(4096);

//     for (int i = 0; i < N; i++) {
//         if (!pipeline->run_once(buf)) {
//             std::cerr << "run_once failed at " << i << std::endl;
//             break;
//         }

//         if (i % 1000 == 0) std::cout << "Processed " << i << " buffers\n";
//     }

//     pipeline->stop();
//     pipeline->shutdown();

//     std::cout << "DONE\n";

//     for (int i = 0; i < 100; i++) {
//         auto p = PipelineBuilder::build_from_yaml("../config/metadata_pipeline.yaml", registry);

//         p->init();
//         p->start();

//         BufferPtr buf = std::make_shared<Buffer>(256);
//         for (int j = 0; j < 100; j++) p->run_once(buf);

//         p->stop();
//         p->shutdown();
//     }

//     std::cout << "DONE 2 \n";
// }

#include "../plugin_test/filter.hpp"
#include "../plugin_test/sink.hpp"
#include "../plugin_test/source.hpp"

int main() {
    auto src  = std::make_unique<Source>();
    auto f    = std::make_unique<Filter>();
    auto sink = std::make_unique<Sink>();

    auto pipeline = std::make_unique<Pipeline>();

    pipeline.get()->link(src.get(), f.get());
    pipeline.get()->link(f.get(), sink.get());

    for (int i = 0; i < 12; ++i) {
        src.get()->push(std::make_shared<Buffer>(1024));
    }

    std::this_thread::sleep_for(std::chrono::seconds(30));
}