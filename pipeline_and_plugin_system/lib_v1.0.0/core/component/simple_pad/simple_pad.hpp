#pragma once
#include "../../unit/pad.hpp"

namespace ViPlugsEngine {

class Element;

class SimplePad : public Pad {
 public:
    SimplePad(PadDirection dir);

    void push(BufferShrPtr buf) override;

    void set_next(std::function<void(BufferShrPtr)> fn);
    bool accept_caps(const Caps&) override;

 public:
    std::function<void(BufferShrPtr)> next;
    std::string                       debug_element;
};

}  // namespace ViPlugsEngine