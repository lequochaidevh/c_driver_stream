// =====================================================
// pipeline.cpp
// =====================================================
#include "pipeline.hpp"

void Pipeline::add(std::unique_ptr<Element> elem) { elements.push_back(std::move(elem)); }

bool Pipeline::init() {  // TODO : Add zero coppy
    for (auto& e : elements) {
        BufferShrPtr in  = std::make_shared<Buffer>(1024);
        BufferShrPtr out = std::make_shared<Buffer>(1024);  // TODO: optimize

        buffers.emplace_back(in);
        buffers.emplace_back(out);  // TODO: optimize

        if (!e->init()) return false;  // element init override
    }

    return true;
}

bool Pipeline::run_once() {
    for (size_t i = 0; i < elements.size(); ++i) {
        if (!elements[i]->process(buffers[i], buffers[i + 1])) return false;
    }
    return true;
}

bool Pipeline::run_once(BufferShrPtr& buf) {
    for (auto& e : elements)
        if (!e->process(buf)) return false;
    return true;
}

void Pipeline::shutdown() {
    for (auto& e : elements) e->shutdown();
}

void Pipeline::stop() {
    for (auto& e : elements) e->stop();
}

bool Pipeline::start() {
    for (auto& e : elements)
        if (!e->start()) return false;
    return true;
}

void Pipeline::link(Element* a, Element* b) {
    auto src  = static_cast<QueuePad*>(a->src_pad());
    auto sink = static_cast<QueuePad*>(b->sink_pad());
    std::cout << "[link] link pad a to b \n";
    src->set_next([sink](auto buf) { sink->push(buf); });
}
