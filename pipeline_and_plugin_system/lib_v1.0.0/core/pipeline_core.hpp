// ===============================
// pipeline_core.hpp
// Buffer + Element core interfaces (shared by core & plugins)
// ===============================
#pragma once

#include "unit/pad.hpp"
#include "component/element.hpp"
// ---------- Plugin C API ----------
extern "C" {
Element *create();
}

// ===============================
// END pipeline_core.hpp
// ===============================
