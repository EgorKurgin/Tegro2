#include <tegro/numerical/matrix.hpp>

namespace tegro {

Matrix::Matrix(std::size_t rows, std::size_t columns)
    : rows_(rows), columns_(columns), data_(rows * columns) {

}

std::size_t Matrix::rows() const {
    return rows_;
}

std::size_t Matrix::columns() const {
    return columns_;
}

double& Matrix::operator()(std::size_t rows, std::size_t columns) {
    return data_[rows * columns_ + columns];
}

const double& Matrix::operator()(std::size_t rows, std::size_t columns) const {
    return data_[rows * columns_ + columns];
}

}