#include "../core/component/element.hpp"
#include "../core/component/queue/queue_pad.hpp"
class Source : public Element {
 public:
    Source() {
        src                                              = std::make_unique<QueuePad>(PadDirection::SRC);
        static_cast<QueuePad*>(src.get())->debug_element = name() + std::string(" src");
    }

    bool push(BufferShrPtr buf) {
        src->push(std::move(buf));
        std::cout << "Source push buffer\n";
        return true;
    }

    void on_buffer(BufferShrPtr) override {}  // unused

    bool init() override {
        std::cout << "[Source] init\n";
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

    void shutdown() override { std::cout << "[Source] shutdown\n"; }

    const char* name() const override { return "Source"; }
};

extern "C" Element* create() { return new Source(); }
