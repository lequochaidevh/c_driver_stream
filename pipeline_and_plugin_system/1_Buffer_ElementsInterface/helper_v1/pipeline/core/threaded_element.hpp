// =====================================================
// STEP B: Pad + Threaded Element
// =====================================================
// threaded_element.hpp
#pragma once

#include "pipeline_core.hpp"
#include "pad.hpp"
#include <thread>
#include <atomic>

class ThreadedElement {
 public:
    ThreadedElement(Element* impl) : impl(impl) {}

    void start() {
        running = true;
        worker  = std::thread(&ThreadedElement::loop, this);
    }

    void stop() {
        running = false;
        if (worker.joinable()) worker.join();
    }

    Pad input;
    Pad output;

 private:
    void loop() {
        impl->init();
        while (running) {
            auto in = input.pull();
            if (!in) continue;
            BufferPtr out;
            impl->process(in, out);
            output.push(out);
        }
        impl->shutdown();
    }

    Element*          impl;
    std::thread       worker;
    std::atomic<bool> running{false};
};

/*
❌ Element tự tạo thread
❌ Element tự giữ queue
❌ Element biết topology

✔ Thread wrapper bên ngoài
✔ Element chỉ biết process buffer
*/