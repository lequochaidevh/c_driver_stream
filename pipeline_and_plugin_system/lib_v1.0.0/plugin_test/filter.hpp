#pragma once
#include "../core/pipeline_core.hpp"
#include "../core/component/queue_pad/queue_pad.hpp"

namespace ViPlugsEngine {
class Filter : public Element {
 public:
    Filter() {
        sink                                              = std::make_unique<QueuePad>(PadDirection::SINK);
        src                                               = std::make_unique<QueuePad>(PadDirection::SRC);
        static_cast<QueuePad*>(sink.get())->debug_element = name() + std::string(" _sink");
        static_cast<QueuePad*>(src.get())->debug_element  = name() + std::string(" src");

        static_cast<QueuePad*>(sink.get())->set_next([this](BufferShrPtr b) { on_buffer(b); });
    }

    bool init() override {
        LOG_DEBUG("Filter init");
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

    void on_buffer(BufferShrPtr buf) override {
        //
        std::cout << "[Filter] on_buffer\n";
        src->push(std::move(buf));
    }

    void shutdown() override { LOG_DEBUG("Filter shutdown"); }

    const char* name() const override { return "Filter"; }
};

}  // namespace ViPlugsEngine