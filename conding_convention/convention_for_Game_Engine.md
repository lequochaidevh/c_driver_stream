Mindset:
 Performance
 Data-oriented
 Cache friendly
 NO fragmentation

1. Naming:
 PascalCase for class
 camelCase for function
 m_ prefix for member

```cpp
class Transform {
public:
    void updateMatrix();

private:
    Vec3 m_position;
    Vec3 m_scale;
};
```

**NOT refer:**
 virtual many in class
 std::list
 deep inheritance

Refer: ECS (Entity Component System)

```cpp
struct Position {
    float x, y, z;
};

struct Velocity {
    float vx, vy, vz;
};
```

Array of structs OR struct of arrays (suit with spec cache pattern.)

2. Memory

Custom allocator:
```cpp
class LinearAllocator {
public:
    void* allocate(size_t size);
    void reset();
};
```

Math
```cpp
inline float dot(const Vec3& a, const Vec3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
```

4. Template Library Style Guide
Mindset:
 Generic
 Header-only
 Compile-time heavy
 Zero-overhead

4.1 Naming:
```txt
 snake_case for function
 type traits: _t, _v
```

```cpp
template <typename T>
struct is_vector : std::false_type {};

template <typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;
```

4.2 Constrain template (C++17 style)

```cpp
template <typename T,
          std::enable_if_t<std::is_integral_v<T>, int> = 0>
T add(T a, T b);
```

4.3 SHOULD NOT:
 virtual
 runtime polymorphism
 heavy cpp file

4.4 constexpr everywhere
```cpp
template <typename T>
constexpr T square(T value) noexcept {
    return value * value;
}
```

4.5 Performance-Oriented C++17 Style
Mindset:
 Zero cost abstraction
 No hidden allocation
 Predictable branch
 Cache efficiency

- Tool benchmark
Use: Google Benchmark library

4.6 Inline hot function
```cpp
inline int fast_add(int a, int b) noexcept {
    return a + b;
}
```

4.7 Avoid virtual

Prefer:
```cpp
template <typename Strategy>
class Processor {
public:
    void run() {
        strategy_.execute();
    }

private:
    Strategy strategy_;
};
```

Prefer Move semantics with true practices:
```cpp
std::vector<int> create() {
    std::vector<int> v{1,2,3};
    return v;  // NRVO
}
```

Reserve before push
```cpp
vec.reserve(1000);
```

4.8 Branch prediction hint
```cpp
if (__builtin_expect(x == 0, 0)) {
}
```

4.9 Struct packing (careful)
```cpp
struct alignas(16) Vec4 {
    float x, y, z, w;
};
```


### TOTAL:

| Style        | Ưu tiên         | Tránh                |
| ------------ | --------------- | -------------------- |
| Embedded     | Deterministic   | heap                 |
| Google       | Maintainability | complexity           |
| Game Engine  | Cache           | OOP deep             |
| Template lib | Compile-time    | runtime polymorphism |
| Performance  | Zero overhead   | abstraction leak     |


