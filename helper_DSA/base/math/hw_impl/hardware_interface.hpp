#pragma once
#include "core/op.hpp"

namespace math {

class HardwareInterface {
 public:
    virtual void execute(const Op& op) = 0;
    virtual ~HardwareInterface()       = default;
};

}  // namespace math