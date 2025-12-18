// =====================================================
// STEP C: Caps & Negotiation
// =====================================================
// caps.hpp
#pragma once

#include "pre_compile_header_file.hpp"

struct Caps {
    std::string format;
    int         width  = 0;
    int         height = 0;
    int         fps    = 0;

    bool compatible(const Caps& other) const {
        return format == other.format && width == other.width && height == other.height && fps == other.fps;
    }
};

// -----------------------------------------------------
// caps negotiation idea (pseudo)
/*
Element A output caps -> Element B input caps
if compatible: link
else: try insert converter element

Logic chuẩn (pseudo)
A.output_caps → B.input_caps

if compatible:
    link
else:
    try insert converter
else:
    fail pipeline


Vì sao Caps KHÓ?

    dynamic change

    renegotiation

    multi-branch

👉 Nhưng nếu bỏ caps, pipeline:

    khó debug

    khó scale

    vendor plugin sẽ vỡ
*/