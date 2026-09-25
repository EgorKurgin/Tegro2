#include <cassert>

#include <tegro/numerical/matrix.hpp>

int matrix_test() {
    tegro::Matrix matrix(2, 3);

    assert(matrix.rows() == 2);
    assert(matrix.columns() == 3);

    matrix(1, 2) = 5;

    assert(matrix(1, 2) == 5);

    return 0;
}