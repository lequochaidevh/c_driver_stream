┌──────────────────────────────┐
│        User API Layer        │  ← matmul(), conv2d(), gemm()
└──────────────┬───────────────┘
               │
┌──────────────▼───────────────┐
│     Math IR / Operator       │  ← Op graph, shape, dtype
└──────────────┬───────────────┘
               │
┌──────────────▼───────────────┐
│   Device-Agnostic Runtime    │  ← scheduler, stream, memory
└──────────────┬───────────────┘
               │
┌───────┬──────▼──────┬────────┐
│ CPU   │   GPU       │  NPU   │  ← backend adapters
│ BLAS  │ CUDA/VK     │ NNAPI  │
└───────┴─────────────┴────────┘

// public
dtype
tensor.hpp              | (dtype)
matrix.hpp (API)        | (tensor)

// implement // entry (todo: factory methods)
matrix_ops.cpp          | matrix.hpp + cpu/implement.hpp

// level 1
op.hpp                  | tensor.hpp
hardware_interface.hpp  | op.hpp

// level 2
// cpu: 
cpu/implement.hpp       | hardware_interface.hpp
cpu/implement.cpp       | cpu/implement.hpp

