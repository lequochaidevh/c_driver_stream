## Follow std cpp rule

1. General
- clarity > cleverness
- dont macro if not need
- RAII helpful

2. Naming Convention
- File name:
```txt
 snake_case.cpp, snack_case.hpp, string_utils.hpp
```
- Namespace:
```cpp
namespace math_utils { }
```

- Class/ Struct (PascalCase):
```cpp
class FileReader;
struct UserInfo;
```
- Function:
```cpp
int calculate_sum(int a, int b);
void print_result();
```

- Variable:
```cpp
int total_count;
std::string user_name;
```

- Private (with " _ " at suffix):
```cpp
class User {
 public:
	std::string get_name() const;

 private:
	std::string name_;
	int age_;
};
```

- Constant (with " k " at prefix):
```cpp
constexpr int kMaxBufferSize = 1024;
```

- Enum:
```cpp
enum class Color {
	red,
	green,
	blue
};
```

3. Formatting
3.1 Indent
- 4 spaces
- Non use TAB

3.2 Braces
```cpp
if (x > 0) {
    do_something();
}
```

3.3 Pointer & Reference style
```cpp
int* ptr;
int& ref;
```

4. Modern C++17 Best Practices

4.1 Use " auto " is suitable.
```
Good: auto it = vec.begin();
Not good: auto x = 5;
```

4.2 Range-based for
```cpp
for (const auto& item : items) {
    process(item);
}
```

4.3 Prefer const
```cpp
void print(const std::string& name);
```

4.4 Use nullptr
```cpp
int* ptr = nullptr;
```

4.5 Use enum class
```cpp
enum class Status { ok, error };
```

4.6 Structured binding (C++17)
```cpp
auto [key, value] = my_pair;
```

4.7 if constexpr
```cpp
template <typename T>
void print_type() {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Integral\n";
    }
}
```

5. Header File Convention

5.1 Header guard
```cpp
#pragma once
```

5.2 Include order

```sh
STL

Third-party

Project headers
```

ex:
```cpp
#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>

#include "string_utils.hpp"
```

6. Class Design Convention.

6.1 Rule of 5 / Rule of 0

- If not manage -> use Rule of 0:
```cpp
class User {
public:
    User(std::string name, int age)
        : name_(std::move(name)), age_(age) {}

    const std::string& name() const { return name_; }
    int age() const { return age_; }

private:
    std::string name_;
    int age_;
};
```

- Use Rule of 5:
```cpp
# Move semantic
```

7. Error Handling
- Priority:
	exception for logic error
	std::optional for missing value
	std::variant for multiple return type
```cpp
std::optional<int> find_id(const std::string& name);
```

8. NOT do:

Using namespace std; in header file
Macro define constant
Raw new/delete
Magic numbers

9. Example <Full Clean C++17 Style>

```cpp
#pragma once

#include <string>
#include <vector>
#include <optional>

namespace user_system {

class User {
public:
    User(std::string name, int age)
        : name_(std::move(name)), age_(age) {}

    const std::string& name() const { return name_; }
    int age() const { return age_; }

private:
    std::string name_;
    int age_;
};

std::optional<User> find_user(const std::vector<User>& users,
                               const std::string& name);

}  // namespace user_system
```

