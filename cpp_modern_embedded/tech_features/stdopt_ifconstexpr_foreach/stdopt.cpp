#include <iostream>
#include <optional>
#include <cstdlib>
#include <ctime>

// simulate sensor
bool sensor_ok() {
    // Random success / failed
    return (std::rand() % 2) == 0;
}

// Return or not anything.
std::optional<int> getTemperature() {
    if (!sensor_ok()) {
        return std::nullopt;  // no value
    }

    return 36;  // temp
}

int main() {
    std::srand(std::time(nullptr));

    auto temp = getTemperature();

    if (temp) {
        std::cout << "Temperature = " << *temp << " C\n";
    } else {
        std::cout << "Failed to read temperature\n";
    }

    // other way: value_or
    int safeTemp = getTemperature().value_or(0);
    std::cout << "1 Safe temperature = " << safeTemp << " C\n";

    // other way: value_or
    safeTemp = *getTemperature();  // can be get INVALID VALUE
    std::cout << "2 Safe temperature = " << safeTemp << " C\n";
    return 0;
}

/*
g++ -std=c++17  stdopt.cpp -o build/a.out

### ./build/a.out
Temperature = 36 C
1 Safe temperature = 0 C
2 Safe temperature = -1164891124 C --------------INVALID VALUE--------------
### ./build/a.out
Failed to read temperature
1 Safe temperature = 0 C
2 Safe temperature = 36 C
### ./build/a.out
Failed to read temperature
1 Safe temperature = 36 C
2 Safe temperature = 36 C
### ./build/a.out
Failed to read temperature
1 Safe temperature = 36 C
2 Safe temperature = 1434434572 C --------------INVALID VALUE--------------
### ./build/a.out
Temperature = 36 C
1 Safe temperature = 36 C
2 Safe temperature = 36 C
###
*/
