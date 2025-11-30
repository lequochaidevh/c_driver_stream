# LOG

```sh
./build_and_run.sh
1 push 
  Constructor MyBuf id = 2
2 push 
  Constructor MyBuf id = 2
  Constructor MyBuf id = 3
1  Destructor MyBuf id = 2
3 push 
  Constructor MyBuf id = 2
  Constructor MyBuf id = 4
2  Destructor MyBuf id = 3
  Constructor MyBuf id = 3
3  Destructor MyBuf id = 2
4 push 
  Constructor MyBuf id = 2
5 push 
  Constructor MyBuf id = 2
  Constructor MyBuf id = 5
4  Destructor MyBuf id = 4
  Constructor MyBuf id = 4
5  Destructor MyBuf id = 3
  Constructor MyBuf id = 3
6  Destructor MyBuf id = 2
  Constructor MyBuf id = 3
7  Destructor MyBuf id = 2
Vector reallocate push_back time = 0.056939 ms
8  Destructor MyBuf id = 1
9  Destructor MyBuf id = 5
10  Destructor MyBuf id = 4
11  Destructor MyBuf id = 3
12  Destructor MyBuf id = 3
13  Destructor MyBuf id = 2
```

# Cụ thể theo từng push
### Push 1

vector rỗng, capacity=0 → cần reallocate lên capacity=1

move constructor chạy 1 lần

destructor chạy 0 lần

### Push 2 

capacity=1 không đủ → reallocate lên capacity=2

move constructor chạy 2 lần (move element cũ + move new)

destructor chạy 1 lần (huỷ buffer cũ)

### Push 3 

capacity=2 không đủ → reallocate lên capacity=4

move constructor chạy 3 lần

destructor chạy 2 lần (huỷ vùng cũ có 2 phần tử)

### Push 4 

capacity=4 còn chỗ → không reallocate

move 1 lần

destructor chạy 0 lần

*** Push 5 ***

capacity=4 không đủ → reallocate lên capacity=8

move constructor chạy 5 lần

destructor chạy 4 lần (huỷ vùng cũ có 4 phần tử)