#include "../core/pipeline_core.hpp"
#include "../core/component/queue_pad/queue_pad.hpp"
#include "../core/component/simple_pad/simple_pad.hpp"

namespace ViPlugsEngine {

class Sink : public Element {
 public:
    Sink() {
        sink = std::make_unique<SimplePad>(PadDirection::SINK);
        static_cast<SimplePad*>(sink.get())->set_next([this](BufferShrPtr b) { on_buffer(b); });
        static_cast<SimplePad*>(sink.get())->debug_element = name() + std::string(" _sink");
    }

    void on_buffer(BufferShrPtr) override { LOG_TRACE("Sink on_buffer"); }

    bool init() override {
        LOG_DEBUG("Sink init");
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

    void shutdown() override { LOG_DEBUG("Sink shutdown"); }

    const char* name() const override { return "Sink"; }
};

extern "C" Element* create() { return new Sink(); }

}  // namespace ViPlugsEngine