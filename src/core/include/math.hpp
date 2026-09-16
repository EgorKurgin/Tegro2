#pragma once

#include <algorithm>
#include <vector>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace tegro {
    // Row-major 2D matrix over std::vector<T>.
    //
    //     Matrix<double> A(3, 3);
    //     A(0, 0) = 1.0;   // top-left
    //     A(2, 2) = 9.0;   // bottom-right
    //
    // Not a linear-algebra type — a container with (i,j) access.
    // Special functions (erf, gamma, bessel, ...) will be added
    // to this header as separate free functions.

    template <typename T>
    class Matrix {
        public:
            Matrix() = default;

            Matrix(std::size_t rows, std::size_t cols)
                  : rows_(rows), cols_(cols), data_(rows * cols, T{}) {}

            Matrix(std::initializer_list<std::initializer_list<T>> init) {
                rows_ = init.size();
                cols_ = (rows_ > 0) ? init.begin()->size() : 0;
                data_.reserve(rows_ * cols_);
                for (const auto& row : init) {
                    if (row.size() != cols_)
                        throw std::invalid_argument("Matrix: inconsistent row size.");
                    data_.insert(data_.end(), row.begin(), row.end());
                }
            };

            [[nodiscard]] std::size_t rows()  const noexcept { return rows_; }
            [[nodiscard]] std::size_t cols()  const noexcept { return cols_; }
            [[nodiscard]] bool        empty() const noexcept { return data_.empty(); }

            T&       operator()(std::size_t i, std::size_t j)       noexcept { return data_[i * cols_ + j]; }
            const T& operator()(std::size_t i, std::size_t j) const noexcept { return data_[i * cols_ + j]; }

            void fill(const T& value) {
                    std::fill(data_.begin(), data_.end(), value);
                }
        private:
            std::size_t    rows_ = 0;
            std::size_t    cols_ = 0;
            std::vector<T> data_;
    };
}