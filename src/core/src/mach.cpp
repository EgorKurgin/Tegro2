#include <linalg.hpp>

#include <algorithm>
#include <cmath>
#include <numeric>
#include <string>
#include <utility>

namespace tegro::linalg {

namespace {
constexpr double kEps  = 1.0e-16;  // pivot tolerance (Gauss-Jordan)
constexpr double kTiny = 1.0e-20;  // zero-pivot substitution (LU)
} // namespace

// =====================================================================
//  LU decomposition, Crout's method with partial pivoting.
//
//  Steps:
//     1. Row scaling factors V[i] = 1 / max|A[i][j]|.
//     2. For each column j:
//        a. Update the sub-diagonal entries A[i][j] for i < j using
//           the already-computed L columns.
//        b. Choose the pivot imax in column j with the largest scaled
//           magnitude, swap rows, adjust sign and scale factor.
//        c. Divide the sub-pivot column by the pivot.
// =====================================================================

bool lu_decompose(Matrix<double>& LU, std::vector<int>& perm, int& sign) {
    const std::size_t n = LU.rows();
    if (LU.cols() != n)
        throw std::invalid_argument("lu_decompose: matrix must be square");

    if (n == 0) {
        perm.clear();
        sign = 1;
        return true;
    }

    perm.resize(n);
    std::iota(perm.begin(), perm.end(), 0);
    sign = 1;

    // Row scaling factors.
    std::vector<double> V(n, 0.0);
    for (std::size_t i = 0; i < n; ++i) {
        double amax = 0.0;
        for (std::size_t j = 0; j < n; ++j)
            amax = std::max(amax, std::abs(LU(i, j)));
        if (amax == 0.0)
            return false;
        V[i] = 1.0 / amax;
    }

    for (std::size_t j = 0; j < n; ++j) {
        // (a) Update A[i][j] for i < j.
        for (std::size_t i = 0; i < j; ++i) {
            double sum = LU(i, j);
            for (std::size_t k = 0; k < i; ++k)
                sum -= LU(i, k) * LU(k, j);
            LU(i, j) = sum;
        }

        // (b) Find pivot in column j.
        double      amax = 0.0;
        std::size_t imax = j;
        for (std::size_t i = j; i < n; ++i) {
            double sum = LU(i, j);
            for (std::size_t k = 0; k < j; ++k)
                sum -= LU(i, k) * LU(k, j);
            LU(i, j) = sum;
            const double dum = V[i] * std::abs(sum);
            if (dum >= amax) {
                amax = dum;
                imax = i;
            }
        }

        // Swap rows if necessary.
        if (j != imax) {
            for (std::size_t k = 0; k < n; ++k)
                std::swap(LU(imax, k), LU(j, k));
            sign = -sign;
            std::swap(V[imax], V[j]);
        }

        perm[j] = static_cast<int>(imax);

        // Zero-pivot substitution: mirror legacy behaviour on singular
        // input rather than throwing.
        if (LU(j, j) == 0.0)
            LU(j, j) = kTiny;

        // (c) Divide by the pivot.
        if (j + 1 < n) {
            const double dum = 1.0 / LU(j, j);
            for (std::size_t i = j + 1; i < n; ++i)
                LU(i, j) *= dum;
        }
    }

    return true;
}

// =====================================================================
//  Solve LU * x = b. Same two-stage substitution as legacy code.
// =====================================================================

void lu_back_solve(const Matrix<double>&   LU,
                   const std::vector<int>& perm,
                   std::vector<double>&    b) {
    const std::size_t n = LU.rows();
    if (b.size() != n)
        throw std::invalid_argument("lu_back_solve: size mismatch");

    // Forward: unscramble the permutation, then substitute.
    int first_nonzero = -1;
    for (std::size_t i = 0; i < n; ++i) {
        const int    ll  = perm[i];
        const double src = b[ll];
        b[ll] = b[i];
        double sum = src;
        if (first_nonzero >= 0) {
            for (std::size_t j = static_cast<std::size_t>(first_nonzero); j < i; ++j)
                sum -= LU(i, j) * b[j];
        } else if (sum != 0.0) {
            first_nonzero = static_cast<int>(i);
        }
        b[i] = sum;
    }

    // Back substitution.
    for (std::size_t i = n; i-- > 0; ) {
        double sum = b[i];
        for (std::size_t j = i + 1; j < n; ++j)
            sum -= LU(i, j) * b[j];
        b[i] = sum / LU(i, i);
    }
}

// =====================================================================
//  Gauss-Jordan inversion with partial pivoting.
//
//  Steps preserved from legacy FastInverse:
//     1. For each column i, find the pivot row i0 with the largest
//        |A[j][i]| for j >= i.
//     2. Swap rows i and i0. Remember the swap in swap[i].
//     3. Normalise row i by 1 / A[i][i].
//     4. Subtract the appropriate multiple of row i from every other
//        row to zero out column i.
//     5. Undo the row swaps by swapping columns back.
// =====================================================================

namespace {

void invert_in_place(Matrix<double>& A) {
    const std::size_t n = A.rows();
    if (A.cols() != n)
        throw std::invalid_argument("invert: matrix must be square");
    if (n == 0)
        return;

    // Closed-form shortcuts for 1x1 and 2x2, kept from the legacy code.
    if (n == 1) {
        if (std::abs(A(0, 0)) < kEps)
            throw SingularMatrix("invert (1x1)");
        A(0, 0) = 1.0 / A(0, 0);
        return;
    }

    if (n == 2) {
        const double a0 = A(0, 0);
        const double b  = a0 * A(1, 1) - A(0, 1) * A(1, 0);
        if (std::abs(b) < kEps)
            throw SingularMatrix("invert (2x2)");
        A(0, 0) =  A(1, 1) / b;
        A(1, 1) =  a0 / b;
        const double nb = -b;
        A(0, 1) /= nb;
        A(1, 0) /= nb;
        return;
    }

    std::vector<int> swap(n, 0);
    for (std::size_t i = 0; i < n; ++i) {
        // 1. Pivot search.
        double      a0  = 0.0;
        std::size_t i0  = i;
        for (std::size_t j = i; j < n; ++j) {
            const double v = std::abs(A(j, i));
            if (v > a0) {
                a0 = v;
                i0 = j;
            }
        }
        if (a0 < kEps)
            throw SingularMatrix("invert (pivot)");

        // 2. Swap rows i and i0.
        swap[i] = static_cast<int>(i0);
        if (i0 != i) {
            for (std::size_t k = 0; k < n; ++k)
                std::swap(A(i, k), A(i0, k));
        }

        // 3. Normalise row i.
        const double inv_pivot = 1.0 / A(i, i);
        for (std::size_t k = 0; k < n; ++k)
            A(i, k) *= inv_pivot;
        A(i, i) = inv_pivot;

        // 4. Eliminate column i from all other rows.
        for (std::size_t j = 0; j < n; ++j) {
            if (j == i) continue;
            const double factor = A(j, i);
            A(j, i) = 0.0;
            for (std::size_t k = 0; k < n; ++k)
                A(j, k) -= factor * A(i, k);
        }
    }

    // 5. Undo row swaps by swapping columns.
    for (std::size_t i = n; i-- > 0; ) {
        const std::size_t j = static_cast<std::size_t>(swap[i]);
        if (j != i) {
            for (std::size_t k = 0; k < n; ++k)
                std::swap(A(k, i), A(k, j));
        }
    }
}

// y = M * x, sizes must match.
void mat_vec_mul(const Matrix<double>&     M,
                 const std::vector<double>& x,
                 std::vector<double>&       y) {
    const std::size_t m = M.rows();
    y.assign(m, 0.0);
    for (std::size_t i = 0; i < m; ++i) {
        double sum = 0.0;
        for (std::size_t k = 0; k < m; ++k)
            sum += M(i, k) * x[k];
        y[i] = sum;
    }
}

} // namespace

Matrix<double> invert(const Matrix<double>& A) {
    Matrix<double> result = A;
    invert_in_place(result);
    return result;
}

// =====================================================================
//  Block-tridiagonal up-down sweep.
//
//  Structure preserved from legacy UpDownStepAdvance:
//     Down-turn (i = n-1 .. 0):
//        Bi <- I + step * Bi,
//        add step * Ci * X[i+1] to X[i],
//        Bi <- Bi - step * Ci (outer) * B[i+1],
//        Bi <- Bi^{-1},
//        X[i] <- Bi * X[i],
//        Bi <- Bi * diag(A[i] * step)   (needed for the up-turn).
//     Up-turn (i = 1 .. n-1):
//        X[i] += Bi * X[i-1].
//  Negative step selects the time-integrated variant.
// =====================================================================

void block_tridiagonal_solve(std::vector<std::vector<double>>& A,
                             std::vector<Matrix<double>>&      B,
                             std::vector<std::vector<double>>& C,
                             std::vector<std::vector<double>>& X,
                             double                            step) {
    const std::size_t n = B.size();
    if (n == 0) return;

    const std::size_t m = B[0].rows();
    for (std::size_t i = 0; i < n; ++i) {
        if (B[i].rows() != m || B[i].cols() != m ||
            A[i].size() != m || C[i].size() != m || X[i].size() != m)
            throw std::invalid_argument(
                "block_tridiagonal_solve: inconsistent dimensions");
    }

    std::vector<double> w(m);

    if (step >= 0.0) {
        for (std::size_t i = n; i-- > 0; ) {
            Matrix<double>& Bi = B[i];

            // Bi <- I + step * Bi
            for (std::size_t r = 0; r < m; ++r) {
                for (std::size_t c = 0; c < m; ++c)
                    Bi(r, c) *= step;
                Bi(r, r) += 1.0;
            }

            if (i + 1 < n) {
                const Matrix<double>& Bnext = B[i + 1];
                for (std::size_t r = 0; r < m; ++r) {
                    const double bb = C[i][r] * step;
                    X[i][r] += bb * X[i + 1][r];
                    for (std::size_t c = 0; c < m; ++c)
                        Bi(r, c) -= bb * Bnext(r, c);
                }
            }

            invert_in_place(Bi);

            mat_vec_mul(Bi, X[i], w);
            X[i] = w;

            if (i > 0) {
                // Bi <- Bi * diag(A[i] * step): scale column r by A[i][r]*step.
                for (std::size_t r = 0; r < m; ++r) {
                    const double bb = A[i][r] * step;
                    for (std::size_t c = 0; c < m; ++c)
                        Bi(c, r) *= bb;
                }
            }
        }
    } else {
        for (std::size_t i = n; i-- > 0; ) {
            Matrix<double>& Bi = B[i];

            if (i + 1 < n) {
                const Matrix<double>& Bnext = B[i + 1];
                for (std::size_t r = 0; r < m; ++r) {
                    const double bb = C[i][r];
                    X[i][r] -= bb * X[i + 1][r];
                    for (std::size_t c = 0; c < m; ++c)
                        Bi(r, c) -= bb * Bnext(r, c);
                }
            }

            invert_in_place(Bi);

            mat_vec_mul(Bi, X[i], w);
            for (auto& v : w) v = -v;
            X[i] = w;

            if (i > 0) {
                for (std::size_t r = 0; r < m; ++r) {
                    const double bb = A[i][r];
                    for (std::size_t c = 0; c < m; ++c)
                        Bi(c, r) *= bb;
                }
            }
        }
    }

    // Up-turn.
    for (std::size_t i = 1; i < n; ++i) {
        const Matrix<double>& Bi = B[i];
        for (std::size_t r = 0; r < m; ++r) {
            double sum = 0.0;
            for (std::size_t c = 0; c < m; ++c)
                sum += Bi(r, c) * X[i - 1][c];
            X[i][r] += sum;
        }
    }
}

} // namespace tegro::linalg