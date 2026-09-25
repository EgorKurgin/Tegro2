#include <tegro/numerical/algorithm.hpp>

#include <cstddef>
#include <cassert>

namespace tegro {

void LU_decomposition(const Matrix& A, Matrix& L, Matrix& U) {
    assert(A.rows() == A.columns());
    assert(L.rows() == L.columns());
    assert(U.rows() == U.columns());
    assert(U.rows() == L.columns());
    assert(U.rows() == A.columns());

    std::size_t n = A.rows();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            U(i, j) = 0;
            L(i, j) = 0;
        }
        L(i, i) = 1;
    }

    auto sum_LU = [&](int i, int j, int limit) {
        double s = 0.0;
        for (int k = 0; k < limit; ++k) {
            s += L(i, k) * U(k, j);
        }
        return s;
    };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i <= j) {
                U(i, j) = A(i, j) - sum_LU(i, j, i);
            } else if (i > j) {
                L(i, j) = (A(i, j) - sum_LU(i, j, j)) / U(j, j);
            }
        }
    }
}
}