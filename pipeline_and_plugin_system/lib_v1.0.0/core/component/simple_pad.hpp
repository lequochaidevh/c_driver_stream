#pragma once
#include "unit/pad.hpp"

class SimplePad : public Pad {
 public:
    void set_next(std::function<void(BufferShrPtr)> cb) { next = std::move(cb); }

    void push(BufferShrPtr buf) override {
        if (next) next(std::move(buf));
    }

 private:
    std::function<void(BufferShrPtr)> next;
};