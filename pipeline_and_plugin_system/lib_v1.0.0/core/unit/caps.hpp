// =====================================================
// STEP C: Caps & Negotiation
// =====================================================
// caps.hpp
#pragma once

#include "pre_compile_header_file.hpp"

namespace ViPlugsEngine {

struct Caps {
    std::string format;
    int         width  = 0;
    int         height = 0;
    int         fps    = 0;

    bool compatible(const Caps& other) const {
        return format == other.format && width == other.width && height == other.height && fps == other.fps;
    }
};

}  // namespace ViPlugsEngine