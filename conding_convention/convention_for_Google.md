## Coding convention used by Google.

1. Naming

| Type     | Style       |
| -------- | ----------- |
| Class    | PascalCase  |
| Function | PascalCase  |
| Variable | snake_case  |
| Member   | snake_case_ |
| Constant | kPascalCase |
| Macro    | ALL_CAPS    |

ex: 
```cpp
class FileReader {
public:
    int ReadFile(const std::string& file_name);

private:
    int buffer_size_;
};
```

2. Google style not refer:
 using namespace std
 Exceptions (internal project)
 Multiple complex inheritance

Good refer:
✔ std::unique_ptr
✔ RAII
✔ const correctness
✔ explicit constructor

```cpp
explicit FileReader(int buffer_size);
```


