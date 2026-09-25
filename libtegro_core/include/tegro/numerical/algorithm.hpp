#pragma once 

#include <tegro/numerical/matrix.hpp>

namespace tegro {

void LU_decomposition(const Matrix& A, Matrix& L, Matrix& U);

}