#include "../core/pipeline_core.hpp"
#include <iostream>

class DebugSink : public Element {
 public:
    bool init() override {
        std::cout << "[DebugSink] init\n";
        return true;
    }

    bool process(const BufferPtr& in, BufferPtr& out) override {
        std::cout << "[DebugSink] got buffer size = " << in->data.size() << std::endl;
        out = in;  // sink Not create new buffer
        return true;
    }

    bool process(BufferPtr& buf) override {
        std::cout << "[DebugSink][no coppy] got buffer size = " << buf->data.size() << std::endl;
        return true;
    }

    void shutdown() override { std::cout << "[DebugSink] shutdown\n"; }

    const char* name() const override { return "DebugSink"; }
};

// extern in order to disable name mangling
// to check: nm -D libdebug_sink.so | grep create -> Create Symbol in ELF:
extern "C" Element* create() { return new DebugSink(); }
// c++
/*
will be name-mangle:
ex: _Z6createv
not have: create
*/