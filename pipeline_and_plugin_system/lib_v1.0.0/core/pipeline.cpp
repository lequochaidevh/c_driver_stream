// =====================================================
// pipeline.cpp
// =====================================================
#include "pipeline.hpp"

void Pipeline::add(std::unique_ptr<Element> elem) { elements.push_back(std::move(elem)); }

bool Pipeline::init() {  // TODO : Add zero coppy
    Element* element_before = nullptr;

    for (auto& e : elements) {
        if (!e->init()) return false;

        if (element_before) {  // skip first source element
            link(element_before, e.get());
        }

        element_before = e.get();
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

bool Pipeline::push_frame(BufferShrPtr frame) {
    if (elements.empty()) return false;
    elements[0].get()->push(frame);
    return false;
}
