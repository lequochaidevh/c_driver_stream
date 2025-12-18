#pragma once

#include "../unit/buffer.hpp"
#include "../unit/pad.hpp"

// ---------- Element Interface ----------
class Element {
 public:
    virtual ~Element()  = default;
    virtual bool init() = 0;
    virtual bool start() { return true; }

    virtual bool process(const BufferShrPtr& in, BufferShrPtr& out) = 0;
    virtual bool process(BufferShrPtr buf)                          = 0;
    virtual void on_buffer(BufferShrPtr)                            = 0;

    virtual bool stop() { return true; }
    virtual void shutdown() = 0;

    Pad* src_pad() { return src.get(); }
    Pad* sink_pad() { return sink.get(); }

    virtual const char* name() const = 0;

    virtual bool push(BufferShrPtr buf) {
        std::cout << "ERROR: can not push src to this element";
        return false;
    };

 protected:
    std::unique_ptr<Pad> src;
    std::unique_ptr<Pad> sink;
};