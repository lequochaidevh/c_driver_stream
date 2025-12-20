// =====================================================
// pad.hpp (PREVIEW – next step)
// =====================================================
// pad.hpp
#pragma once

#include "caps.hpp"
#include "buffer.hpp"
namespace ViPlugsEngine {

enum class PadDirection { SRC, SINK };

class Pad {
 public:
    Pad(PadDirection dir) : direction(dir) {}
    virtual ~Pad() = default;

    virtual void push(BufferShrPtr buf) = 0;

    PadDirection direction;

    virtual bool accept_caps(const Caps&) = 0;
    Caps         caps;
};

}  // namespace ViPlugsEngine