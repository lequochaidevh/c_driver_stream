# C++17 Coding & Naming Convention

### Team Standard v1.0

0. Core Principles
0.1. Consistency > personal preference
0.2. One symbol type → one naming style
0.3. No mixed styles
0.4. Clarity > cleverness
0.5. No unnecessary abbreviations

1. File Naming
```sh
snake_case.hpp
snake_case.cpp
# Examples:
memory_pool.hpp
http_server.cpp
entity_manager.hpp
# Test:
memory_pool_test.cpp
```
2. Namespace
```cpp
namespace engine {
namespace memory {
}
}
```
Rules:
 No PascalCase namespaces
 No unclear abbreviations

3. Types
3.1. Class / Struct / Union
 ```cpp
 class MemoryPool;
 struct Vec3;
 ```

3.2. Enum
 ```cpp
 enum class ErrorCode {
     ok,
     invalid_argument,
     timeout
 };
 ```

 Rules:
  Use `enum` class
  Enum values must be `snake_case`

4. Functions
```cpp
void initialize();
int calculate_score();
std::string to_string() const;
```

Boolean Functions start with:
```cpp
is_
has_
can_
should_
// ex:
bool is_valid() const;
bool has_value() const;
bool can_allocate() const;

```

5. Variables
5.1. Local Variables
 ```cpp
 int total_count;
 float delta_time;
 void set_position(float x, float y);
 ```
5.2

6. Member Variables (Mandatory Rule)
```cpp
snake_case_
class Player {
private:
    int health_;
    float speed_;
};
```

Forbidden
```cpp
m_health
_health
__health
```
Reason:
`_x` and `__x` are reserved identifiers
`m_` is not allowed in this codebase

7. Constants
```cpp
kPascalCase
constexpr int kMaxBufferSize = 1024;
constexpr float kPi = 3.1415926f;
```
7.2 Global Constants
```cpp
inline constexpr int kDefaultPort = 8080;
```

8. Templates
8.1 Template Type Parameters
PascalCase
```cpp
template <typename Key, typename Value>
```

8.2 Policy / Strategy Types
Must describe role clearly:
```cpp
template <typename AllocationStrategy>
template <typename HashPolicy>
```

**Avoid:**
```cpp
template <typename T1, typename T2>
```

8.3 Trait Naming
```cpp
is_xxx
has_xxx
```
Suffix rules:
```sh
_t  → type alias
_v  → constexpr value
```
Example:
```cpp
template <typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

template <typename T>
struct add_pointer {
    using type = T*;
};
// Old:
typename add_pointer<int>::type   // int*

// New:
template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

// Use:
add_pointer_t<int>   // int*

// _v = value (constexpr value)
template <typename T>
struct is_int {
    static constexpr bool value = false;
};
template <>
struct is_int<int> {
    static constexpr bool value = true;
};
// Create alias
template <typename T>
inline constexpr bool is_int_v = is_int<T>::value;
```

9. Type Aliases
PascalCase
```cpp
using StringList = std::vector<std::string>;
using IdType = std::uint64_t;
```
10. Macros
ALL_CAPS_WITH_UNDERSCORE
```cpp
#define ENGINE_DEBUG 1

// Header guard:

#ifndef ENGINE_MEMORY_POOL_HPP
#define ENGINE_MEMORY_POOL_HPP
```
11. Global Variables

Avoid global variables.

If absolutely required:
```cpp
g_snake_case
int g_frame_counter;
```
12. Smart Pointers

Do NOT encode pointer type in name.
Correct:
```cpp
std::unique_ptr<Player> player;
std::shared_ptr<Texture> texture;
```

Incorrect:
```cpp
player_ptr
texture_sp
```

13. Concurrency Objects
Names must reflect purpose:
```cpp
std::atomic<bool> is_running_;
std::mutex state_mutex_;
```

14. Semantic Naming Rules
14.1 Do NOT encode type in name

Incorrect:
```cpp
str_name
i_count
```

Correct:
```cpp
name
count
```

14.2 Avoid unclear abbreviations

Forbidden:
```cpp
cfg
mgr
tmp
```
Use:
```cpp
config
manager
temporary_buffer
```

14.3 Do NOT name by container type

Incorrect:
```cpp
std::vector<int> int_list;
```
Correct:
```cpp
std::vector<int> scores;
```

15. Example (Fully Compliant)
```cpp
namespace engine {

class MemoryPool {
public:
    explicit MemoryPool(std::size_t block_size);

    void* allocate();
    void deallocate(void* pointer);

    bool is_empty() const noexcept;

private:
    void initialize_blocks();

    std::size_t block_size_;
    std::size_t free_count_;
};

}  // namespace engine
```

16. Code Review Enforcement

Naming violations must be fixed before merge.
No style debates after adoption of this document.
Changes to this rule require team agreement and document update.
Consistency across modules is mandatory.

17. Summary Table
Symbol	Rule
File	snake_case
Namespace	snake_case
Class / Struct	PascalCase
Function	snake_case
Variable	snake_case
Member	snake_case_
Constant	kPascalCase
Enum value	snake_case
Template type	PascalCase
Macro	ALL_CAPS
