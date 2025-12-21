#include "../core/pipeline_core.hpp"
#include "../core/component/queue/queue_pad.hpp"

namespace ViPlugsEngine {

class InvertFilter : public Element {
 public:
    InvertFilter() {
        sink                                              = std::make_unique<QueuePad>(PadDirection::SINK);
        src                                               = std::make_unique<QueuePad>(PadDirection::SRC);
        static_cast<QueuePad*>(sink.get())->debug_element = name() + std::string(" _sink");
        static_cast<QueuePad*>(src.get())->debug_element  = name() + std::string(" src");

        static_cast<QueuePad*>(sink.get())->set_next([this](BufferShrPtr b) { on_buffer(b); });
    }

    bool init() override {
        LOG_DEBUG("InvertFilter init");
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

    void set_downstream(std::function<void(std::shared_ptr<Buffer>)> fn) { downstream = fn; }

    void on_buffer(BufferShrPtr buf) override {
        for (auto& b : buf->data) b = 255 - b;
        // heavy work
        // std::this_thread::sleep_for(std::chrono::milliseconds(33));
        LOG_DEBUG("InvertFilter on_buffer");
        src->push(std::move(buf));
    }

    void shutdown() override { LOG_DEBUG("InvertFilter shutdown"); }

    const char* name() const override { return "InvertFilter"; }

 private:
    std::function<void(std::shared_ptr<Buffer>)> downstream;
};

extern "C" Element* create() { return new InvertFilter(); }

}  // namespace ViPlugsEngine

/*

Plugin NOT keep state global

g++ -fPIC -shared invert_filter.cpp \
    -I../core \
    -L../core -lpipeline_core \
    -o libinvert_filter.so
*/