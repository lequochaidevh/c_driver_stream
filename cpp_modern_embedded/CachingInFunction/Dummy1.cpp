#include <iostream>

enum class DeviceType { IMAGE, VIDEO, AUDIO, UNKNOWN };

namespace MediaDevice {

// Dummy function for testing
DeviceType detect_device_type() {
  // This log proves whether the function actually runs or uses the cache
  std::cout << "[LOG] Detecting device type from hardware..." << std::endl;
  return DeviceType::VIDEO;
}

DeviceType get_device_type() {
  static const DeviceType cache = detect_device_type();
  return cache;
}
} // namespace MediaDevice

int main() {
  std::cout << "--- Call 1 ---" << std::endl;
  MediaDevice::get_device_type(); // Should print the [LOG]

  std::cout << "\n--- Call 2 ---" << std::endl;
  MediaDevice::get_device_type(); // Should NOT print the [LOG] (uses cache)

  std::cout << "\n--- Call 3 ---" << std::endl;
  if (DeviceType::VIDEO == MediaDevice::get_device_type()) {
    std::cout << "\n--- Device is video ---" << std::endl;
  }
  // Should NOT print the [LOG] (uses cache)

  return 0;
}
