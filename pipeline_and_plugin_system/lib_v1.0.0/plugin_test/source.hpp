#pragma once
#include "../core/pipeline_core.hpp"
#include "../core/component/queue_pad/queue_pad.hpp"

namespace ViPlugsEngine {

class Source : public Element {
 public:
    Source() {
        src                                              = std::make_unique<QueuePad>(PadDirection::SRC);
        static_cast<QueuePad*>(src.get())->debug_element = name() + std::string(" src");
    }

    bool push(BufferShrPtr buf) {
        src->push(std::move(buf));
        LOG_DEBUG("Source push");
    }

    void on_buffer(BufferShrPtr) override {}  // unused

    bool init() override {
        LOG_DEBUG("Source init");
        return true;
    }

    bool process(const BufferShrPtr& in, BufferShrPtr& out) override {
        if (!in) return false;
        out = std::make_shared<Buffer>(*in);
        for (auto& px : out->data) px = 255 - px;
        return true;
    }

    bool process(BufferShrPtr buf) override {
        for (auto& b : buf->data) b = 255 - b;
        src->push(std::move(buf));

        return true;
    }

    void shutdown() override { LOG_DEBUG("Source shutdown"); }

    const char* name() const override { return "Source"; }
};

}  // namespace ViPlugsEngine