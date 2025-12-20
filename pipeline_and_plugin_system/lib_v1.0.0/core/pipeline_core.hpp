// ===============================
// pipeline_core.hpp
// Buffer + Element core interfaces (shared by core & plugins)
// ===============================
#pragma once

#include "unit/pad.hpp"
#include "component/element.hpp"
// ---------- Plugin C API ----------

namespace ViPlugsEngine {

class Environment {
 public:
    static void init();
};

extern "C" {
Element *create();
}
}  // namespace ViPlugsEngine
// ===============================
// END pipeline_core.hpp
// ===============================
