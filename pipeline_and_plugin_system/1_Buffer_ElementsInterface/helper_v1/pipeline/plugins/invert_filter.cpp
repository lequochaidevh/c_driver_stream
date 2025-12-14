#include "../core/pipeline_core.hpp"
#include <iostream>

class InvertFilter : public Element {
 public:
    bool init() override {
        std::cout << "[InvertFilter] init\n";
        return true;
    }

    bool process(const BufferPtr& in, BufferPtr& out) override {
        if (!in) return false;
        out = std::make_shared<Buffer>(*in);
        for (auto& px : out->data) px = 255 - px;
        return true;
    }
    bool process(BufferPtr& buf) override {
        if (!buf) return false;
        BufferPtr newBuf;
        newBuf = std::make_shared<Buffer>(*buf);
        for (auto& px : newBuf->data) px = 255 - px;
        return true;
    }
    void shutdown() override { std::cout << "[InvertFilter] shutdown\n"; }

    const char* name() const override { return "InvertFilter"; }
};

extern "C" Element* create() { return new InvertFilter(); }

/*

Plugin NOT keep state global

g++ -fPIC -shared invert_filter.cpp \
    -I../core \
    -L../core -lpipeline_core \
    -o libinvert_filter.so
*/