#pragma once

namespace math {

enum class DType {
    F32,
};

inline size_t dtype_size(DType t) {
    switch (t) {
        case DType::F32: return 4;
    }
    return 0;
}

}