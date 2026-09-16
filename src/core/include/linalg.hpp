#pragma once

#include <math.hpp>

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace tegro::linalg {

// Thrown when a matrix is found to be singular during inversion
// or during a block-solve pivot step.
class SingularMatrix : public std::runtime_error {
public:
    explicit SingularMatrix(const char* where)
        : std::runtime_error(std::string("singular matrix in ") + where) {}
};

// ---------------------------------------------------------------------
// LU decomposition, Crout's method with partial pivoting, in place.
//
// On success:
//   - LU holds the combined L (below diagonal, unit diagonal implied)
//     and U (on and above diagonal).
//   - perm[i] is the row originally at position i after pivoting.
//   - sign is +1 or -1 depending on the parity of row swaps.
//
// Returns false if a zero pivot is found.
// ---------------------------------------------------------------------
bool lu_decompose(Matrix<double>& LU, std::vector<int>& perm, int& sign);

// Solve LU * x = b for x using the output of lu_decompose.
// b is overwritten with the solution.
void lu_back_solve(const Matrix<double>&   LU,
                   const std::vector<int>& perm,
                   std::vector<double>&    b);

// ---------------------------------------------------------------------
// Invert a square matrix by Gauss-Jordan elimination with partial
// pivoting.
//
// Throws SingularMatrix if A is singular.
// ---------------------------------------------------------------------
Matrix<double> invert(const Matrix<double>& A);

// ---------------------------------------------------------------------
// Solve a block-tridiagonal system of the form
//
//     A[i] * X[i-1]  -  B[i] * X[i]  +  C[i] * X[i+1]  =  f[i]
//
// for i = 0 .. n-1, where:
//     A[i], C[i]  — diagonal matrices, stored as vectors of length m
//                   (A[0] and C[n-1] are not referenced),
//     B[i]        — dense m x m matrix,
//     X[i]        — vector of length m; on input holds f[i],
//                   on output holds the solution x[i].
//
// The parameter `step` enters the implicit Euler scheme:
//     B[i] is replaced in place by  I + step * B[i]
// on the diagonal. A negative `step` selects the time-integrated
// variant.
//
// Throws SingularMatrix if a pivot fails during block inversion.
// ---------------------------------------------------------------------
void block_tridiagonal_solve(std::vector<std::vector<double>>& A,
                             std::vector<Matrix<double>>&      B,
                             std::vector<std::vector<double>>& C,
                             std::vector<std::vector<double>>& X,
                             double                            step);

} // namespace tegro::linalg