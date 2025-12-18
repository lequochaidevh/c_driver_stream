#pragma once
#include "../core/component/element.hpp"
#include "../core/component/queue/queue_pad.hpp"

class Sink : public Element {
 public:
    Sink() {
        sink = std::make_unique<QueuePad>(PadDirection::SINK);
        static_cast<QueuePad*>(sink.get())->set_next([this](BufferShrPtr b) { on_buffer(b); });
        static_cast<QueuePad*>(sink.get())->debug_element = name() + std::string(" _sink");
    }

    void on_buffer(BufferShrPtr) override { std::cout << " on_buffer Sink got buffer\n"; }

    bool init() override {
        std::cout << "[Sink] init\n";
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

    void shutdown() override { std::cout << "[Sink] shutdown\n"; }

    const char* name() const override { return "Sink"; }
};