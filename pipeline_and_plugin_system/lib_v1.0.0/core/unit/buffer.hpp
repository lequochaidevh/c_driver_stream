#pragma once
#include "pre_compile_header_file.hpp"
#include "metadata.hpp"

// ---------- Buffer ----------
struct Buffer {
    uint64_t             pts = 0;
    std::vector<uint8_t> data;
    MetaData             meta;

    Buffer() = default;

    explicit Buffer(size_t size) : data(size) {}
};

using BufferShrPtr = std::shared_ptr<Buffer>;
