## Coding convention for embedded

1. Compiler flag
-fno-exceptions
-fno-rtti
-Os or -O2

2. Naming like std
3. Feature stdlib using
Not use or not prefer:
```cpp
new/delete
std::vector (exclude custom allocator static)
std::string (more heap)
exception
iostream
```
Prefer: 
```cpp
 std::array
 std::span (C++20 nếu có)
 constexpr
 enum class
 fixed-width types
```
ex:

```cpp
#include <cstdint>

class UartDriver {
public:
    void init(uint32_t baud_rate) noexcept;
    void send(uint8_t byte) noexcept;

private:
    volatile uint32_t* base_addr_;
};
```

4. Memory style

Prefer:
```cpp
std::array<uint8_t, 128> buffer_;
```

Not prefer:
```cpp
std::vector<uint8_t>
```

5. Error handling
use enum return;
```cpp
enum class ErrorCode {
    ok,
    timeout,
    invalid_param
};

ErrorCode init() noexcept;
```

6. ISR rule

NOT dynamic memory
NOT lock
NOT printf
-> ONLY set flag


