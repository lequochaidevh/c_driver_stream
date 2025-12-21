#include "../../logger/logger.hpp"
#include "simple_pad.hpp"

namespace ViPlugsEngine {

SimplePad::SimplePad(PadDirection dir) : Pad(dir) {}

void SimplePad::push(BufferShrPtr buf) {
    if (next) {
        CORE_LOG_TRACE("simple pad of element: {} ", debug_element);
        next(std::move(buf));
    }
    return;
}

void SimplePad::set_next(std::function<void(BufferShrPtr)> fn) { next = std::move(fn); }

bool SimplePad::accept_caps(const Caps &) { return false; }

}  // namespace ViPlugsEngine