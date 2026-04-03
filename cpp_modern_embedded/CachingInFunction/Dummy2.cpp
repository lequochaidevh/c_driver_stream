#include <iostream>
#include <string>
#include <unordered_map>

class Device {
private:
  // Dummy function simulating a slow database or hardware lookup
  static int detect_device_id_with_name(const std::string &name) {
    std::cout << "[LOG] Searching hardware for: " << name << " --- ";
    if (name == "camera")
      return 101;
    if (name == "mic")
      return 202;
    if (name == "video")
      return 303;
    return 0; // Unknown
  }

public:
  // Belong to Class
  int get_device_id_with_name(const std::string &name) {
    // This map is created once and persists for the life of the program
    static std::unordered_map<std::string, int> cache;

    // Check if we already looked up this specific name
    auto it = cache.find(name);
    if (it != cache.end()) {
      return it->second; // Return cached ID
    }

    // Not in cache, so detect it and save it
    int id = detect_device_id_with_name(name);
    cache[name] = id;
    return id;
  }
};

int main() {

  std::cout << "--- Test Case 1 -----------------------------------------"
            << std::endl;
  Device obj1;

  std::cout << "--- First call (camera) ---" << std::endl;
  std::cout << "ID: " << obj1.get_device_id_with_name("camera")
            << std::endl; // New [LOG] here

  std::cout << "\n--- Second call (camera) ---" << std::endl;
  std::cout << "ID: " << obj1.get_device_id_with_name("camera")
            << std::endl; // No [LOG] here

  std::cout << "\n--- First call (mic) ---" << std::endl;
  std::cout << "ID: " << obj1.get_device_id_with_name("mic")
            << std::endl; // New [LOG] here

  //------------------------------------------------------------------------------------------
  std::cout << "--- Test Case 2 -----------------------------------------"
            << std::endl;
  Device obj2;

  std::cout << "\n--- OBJ 2: Second call (camera) ---" << std::endl;
  // obj2 local(this) get
  std::cout << "ID: " << obj2.get_device_id_with_name("camera")
            << std::endl; // No [LOG] here

  // obj2 static get
  std::cout << "ID: " << obj2.get_device_id_with_name("video")
            << std::endl; // New [LOG] here

  // obj1 static get
  std::cout << "ID: " << obj1.get_device_id_with_name("video")
            << std::endl; // No [LOG] here

  //------------------------------------------------------------------------------------------
  return 0;
}
