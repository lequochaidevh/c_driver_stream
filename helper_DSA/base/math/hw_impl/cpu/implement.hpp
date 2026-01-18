#pragma once
#include "hw_impl/hardware_interface.hpp"

namespace math {

class CPU : public HardwareInterface {
 public:
    void execute(const Op& op) override;
};

}  // namespace math