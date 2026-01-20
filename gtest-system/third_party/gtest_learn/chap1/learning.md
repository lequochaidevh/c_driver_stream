## What can one learn from this lesson?
* Init setup env.
* Flag compile
* Setup cpp, and how can use assert TRUE and FALSE

**Result:**
```text
./test.out 
[==========] Running 2 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 1 test from TestName1
[ RUN      ] TestName1.Subtest1
assert_test.cpp:6: Failure
Value of: 1 == 2
  Actual: false
Expected: true

[  FAILED  ] TestName1.Subtest1 (0 ms)
[----------] 1 test from TestName1 (0 ms total)

[----------] 1 test from TestName2
[ RUN      ] TestName2.Subtest2
[       OK ] TestName2.Subtest2 (0 ms)
[----------] 1 test from TestName2 (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 2 test suites ran. (0 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] TestName1.Subtest1

```