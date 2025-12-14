// =====================================================
// pad.hpp (PREVIEW – next step)
// =====================================================
#pragma once

#include "pipeline_core.hpp"
#include <queue>

class Pad {
 public:
    void      push(const BufferPtr& buf) { queue.push(buf); }
    BufferPtr pull() {
        if (queue.empty()) return nullptr;
        auto b = queue.front();
        queue.pop();
        return b;
    }

 private:
    std::queue<BufferPtr> queue;
};