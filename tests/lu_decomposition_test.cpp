#include <cassert>

#include <tegro/numerical/algorithm.hpp>

int algorithm_test() {
    tegro::Matrix A(2, 3);
    tegro::Matrix L(2, 2);
    tegro::Matrix U(2, 2);

    LU_decomposition(A, L, U);

    return 0;
}