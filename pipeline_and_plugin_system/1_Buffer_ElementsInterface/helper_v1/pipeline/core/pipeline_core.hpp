// ===============================
// pipeline_core.hpp
// Buffer + Element core interfaces (shared by core & plugins)
// ===============================
#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <string>

// ---------- Metadata ----------
struct MetaData {
    uint32_t width  = 0;
    uint32_t height = 0;
    uint32_t format = 0;
};

// ---------- Buffer ----------
struct Buffer {
    uint64_t             pts = 0;
    std::vector<uint8_t> data;
    MetaData             meta;

    Buffer() = default;

    explicit Buffer(size_t size) : data(size) {}
};

using BufferPtr = std::shared_ptr<Buffer>;

// ---------- Element Interface ----------
class Element {
 public:
    virtual ~Element()  = default;
    virtual bool init() = 0;
    virtual bool start() { return true; }
    virtual bool process(const BufferPtr& in, BufferPtr& out) = 0;
    virtual bool process(BufferPtr& buf)                      = 0;
    virtual bool stop() { return true; }
    virtual void shutdown() = 0;

    virtual const char* name() const = 0;
};

// ---------- Plugin C API ----------
extern "C" {
Element* create();
}

// ===============================
// END pipeline_core.hpp
// ===============================
