// =====================================================
// pipeline.hpp
// Linear pipeline executor (step 1)
// =====================================================
#pragma once

#include "pipeline_core.hpp"
#include <vector>

class Pipeline {
 public:
    void add(Element* e);
    bool init();
    bool start();
    bool run_once();
    bool run_once(BufferPtr& buf);
    void shutdown();
    void stop();

 private:
    std::vector<Element*>  elements;
    std::vector<BufferPtr> buffers;
};
