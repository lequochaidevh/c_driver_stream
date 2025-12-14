// =====================================================
// pipeline.cpp
// =====================================================
#include "pipeline.hpp"

void Pipeline::add(std::unique_ptr<Element> elem) { elements.push_back(std::move(elem)); }

bool Pipeline::init() {  // TODO : Add zero coppy
    // buffers.resize(elements.size() + 1);
    for (auto& e : elements) {
        BufferPtr in  = std::make_shared<Buffer>(1024);
        BufferPtr out = std::make_shared<Buffer>(1024);  // TODO: optimize

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

bool Pipeline::run_once(BufferPtr& buf) {
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