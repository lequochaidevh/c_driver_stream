# TEST SYSTEM v1.0.0

# Prerequisite
- Python 3.8.10
+ yaml

```text
test-system/
├── cmake/
│   ├── toolchain.cmake
│   └── test_common.cmake
│
├── tools/                  # Tool create test
│   ├── gen_unit_test.py
│   ├── gen_integration.py
│   └── test_manifest.yaml
│
├── opensrc/                # TEST OPEN SRC (debug)
│   ├── CMakeLists.txt
│   ├── include/
│   ├── src/                # copy / submodule lib opensrc
│   └── tests/
│       ├── unit/
│       └── integration/
│
├── closesrc/               # TEST CLOSE SRC (binary)
│   ├── CMakeLists.txt
│   ├── include/            # public headers
│   ├── lib/                # *.a / *.so
│   └── tests/
│       ├── unit/
│       └── integration/
│
├── common/                 # common using
│   ├── test_utils/
│   └── mock/
│
└── README.md
```