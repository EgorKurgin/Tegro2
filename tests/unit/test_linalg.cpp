#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <math.hpp>
#include <linalg.hpp>

#include <vector>

using namespace tegro;
using Catch::Approx;

// ---------------------------------------------------------------------
// lu_decompose + lu_back_solve
// ---------------------------------------------------------------------

TEST_CASE("linalg: LU on 2x2 system", "[linalg][lu]") {
    // A = [[2, 1], [1, 2]],  b = [3, 3],  x = [1, 1].
    Matrix<double> LU{
        {2.0, 1.0},
        {1.0, 2.0},
    };
    std::vector<int> perm;
    int              sign = 1;
    REQUIRE(linalg::lu_decompose(LU, perm, sign));

    std::vector<double> b = {3.0, 3.0};
    linalg::lu_back_solve(LU, perm, b);

    REQUIRE(b[0] == Approx(1.0));
    REQUIRE(b[1] == Approx(1.0));
}

TEST_CASE("linalg: LU on 3x3 system", "[linalg][lu]") {
    // A = [[2,-1, 0],[-1, 2,-1],[0,-1, 2]], b = [1, 0, 1], x = [1, 1, 1].
    Matrix<double> LU{
        { 2.0, -1.0,  0.0},
        {-1.0,  2.0, -1.0},
        { 0.0, -1.0,  2.0},
    };
    std::vector<int> perm;
    int              sign = 1;
    REQUIRE(linalg::lu_decompose(LU, perm, sign));

    std::vector<double> b = {1.0, 0.0, 1.0};
    linalg::lu_back_solve(LU, perm, b);

    REQUIRE(b[0] == Approx(1.0));
    REQUIRE(b[1] == Approx(1.0));
    REQUIRE(b[2] == Approx(1.0));
}

TEST_CASE("linalg: LU reports zero-row matrix", "[linalg][lu]") {
    Matrix<double> LU{
        {0.0, 0.0},
        {0.0, 0.0},
    };
    std::vector<int> perm;
    int              sign = 1;
    REQUIRE_FALSE(linalg::lu_decompose(LU, perm, sign));
}

// ---------------------------------------------------------------------
// invert
// ---------------------------------------------------------------------

TEST_CASE("linalg: invert 2x2", "[linalg][inverse]") {
    Matrix<double> A{
        {2.0, 1.0},
        {1.0, 2.0},
    };
    const Matrix<double> inv = linalg::invert(A);

    REQUIRE(inv(0, 0) == Approx( 2.0 / 3.0));
    REQUIRE(inv(0, 1) == Approx(-1.0 / 3.0));
    REQUIRE(inv(1, 0) == Approx(-1.0 / 3.0));
    REQUIRE(inv(1, 1) == Approx( 2.0 / 3.0));
}

TEST_CASE("linalg: invert 3x3 diagonal", "[linalg][inverse]") {
    Matrix<double> A{
        {1.0, 0.0, 0.0},
        {0.0, 2.0, 0.0},
        {0.0, 0.0, 4.0},
    };
    const Matrix<double> inv = linalg::invert(A);

    REQUIRE(inv(0, 0) == Approx(1.0));
    REQUIRE(inv(1, 1) == Approx(0.5));
    REQUIRE(inv(2, 2) == Approx(0.25));
}

TEST_CASE("linalg: invert throws on singular", "[linalg][inverse]") {
    Matrix<double> A{
        {1.0, 2.0},
        {2.0, 4.0},
    };
    REQUIRE_THROWS_AS(linalg::invert(A), linalg::SingularMatrix);
}

TEST_CASE("linalg: A * A^-1 == I", "[linalg][inverse]") {
    Matrix<double> A{
        { 4.0,  1.0,  2.0},
        { 1.0,  5.0,  3.0},
        { 2.0,  3.0,  6.0},
    };
    const Matrix<double> inv = linalg::invert(A);

    const std::size_t n = 3;
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            double sum = 0.0;
            for (std::size_t k = 0; k < n; ++k)
                sum += A(i, k) * inv(k, j);
            const double expected = (i == j) ? 1.0 : 0.0;
            REQUIRE(sum == Approx(expected).margin(1e-10));
        }
    }
}

// ---------------------------------------------------------------------
// block_tridiagonal_solve
// ---------------------------------------------------------------------

TEST_CASE("linalg: block tridiag on 1-block scalar system",
          "[linalg][block]") {
    // n = 1 block, m = 1. Equation: -B'[0] * x[0] = f[0] with
    // B'[0] = 1 + step * B[0]. With B[0] = 2, f[0] = 3, step = 1:
    // B'[0] = 3, x[0] = -1.
    std::vector<std::vector<double>> A = {{0.0}};
    std::vector<Matrix<double>>      B = {Matrix<double>{{2.0}}};
    std::vector<std::vector<double>> C = {{0.0}};
    std::vector<std::vector<double>> X = {{3.0}};

    linalg::block_tridiagonal_solve(A, B, C, X, 1.0);

    REQUIRE(X[0][0] == Approx(-1.0));
}

TEST_CASE("linalg: block tridiag on 2-block scalar system",
          "[linalg][block]") {
    // n = 2, m = 1.
    //
    // Equations (step = 1, so B'[i] = 1 + B[i]):
    //     -B'[0] * x[0] + C[0] * x[1] = f[0]
    //      A[1] * x[0] - B'[1] * x[1] = f[1]
    //
    // Data: B[0] = 2, B[1] = 1, C[0] = 1, A[1] = 1, f = [1, 1].
    // B'[0] = 3, B'[1] = 2.
    //
    // Solve:
    //     -3 x0 +  x1 = 1
    //      x0 - 2 x1 = 1
    //
    // From 2nd: x0 = 1 + 2 x1.
    // Substitute: -3(1 + 2 x1) + x1 = 1 => -5 x1 = 4
    //                                  => x1 = -0.8, x0 = -0.6.
    std::vector<std::vector<double>> A = {{0.0}, {1.0}};
    std::vector<Matrix<double>>      B = {
        Matrix<double>{{2.0}},
        Matrix<double>{{1.0}},
    };
    std::vector<std::vector<double>> C = {{1.0}, {0.0}};
    std::vector<std::vector<double>> X = {{1.0}, {1.0}};

    linalg::block_tridiagonal_solve(A, B, C, X, 1.0);

    REQUIRE(X[0][0] == Approx(-0.6));
    REQUIRE(X[1][0] == Approx(-0.8));
}

TEST_CASE("linalg: block tridiag with 2x2 blocks",
          "[linalg][block]") {
    // n = 2 blocks, m = 2.
    //
    // B[0] = I, B[1] = I, step = 1  =>  B'[i] = 2 I.
    // A[1] = (1, 1), C[0] = (1, 1).
    // f[0] = (2, 2), f[1] = (2, 2).
    //
    // Per-component equations:
    //     -2 x0 + 1 x1 = 2
    //      1 x0 - 2 x1 = 2
    //
    // Solve: x0 = 2 + 2 x1
    //        -2(2 + 2 x1) + x1 = 2 => -3 x1 = 6 => x1 = -2, x0 = -2.
    std::vector<std::vector<double>> A = {{0.0, 0.0}, {1.0, 1.0}};
    std::vector<Matrix<double>>      B = {
        Matrix<double>{ {1.0, 0.0}, {0.0, 1.0} },
        Matrix<double>{ {1.0, 0.0}, {0.0, 1.0} },
    };
    std::vector<std::vector<double>> C = {{1.0, 1.0}, {0.0, 0.0}};
    std::vector<std::vector<double>> X = {{2.0, 2.0}, {2.0, 2.0}};

    linalg::block_tridiagonal_solve(A, B, C, X, 1.0);

    REQUIRE(X[0][0] == Approx(-2.0));
    REQUIRE(X[0][1] == Approx(-2.0));
    REQUIRE(X[1][0] == Approx(-2.0));
    REQUIRE(X[1][1] == Approx(-2.0));
}