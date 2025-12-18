// =====================================================
// pipeline.hpp
// Linear pipeline executor (step 1)
// =====================================================
#pragma once

#include "pipeline_core.hpp"
#include "../core/component/queue/queue_pad.hpp"

class Pipeline {
 public:
    Pipeline()  = default;
    ~Pipeline() = default;

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    Pipeline(Pipeline&&) = default;
    Pipeline& operator=(Pipeline&&) = default;

 public:
    void add(std::unique_ptr<Element> elem);
    bool init();
    bool start();
    bool run_once();
    bool run_once(BufferShrPtr& buf);
    void shutdown();
    void stop();

    void link(Element* a, Element* b);

    bool push_frame(BufferShrPtr frame);

 private:
    std::vector<std::unique_ptr<Element>> elements;
    std::vector<BufferShrPtr>             buffers;
};
