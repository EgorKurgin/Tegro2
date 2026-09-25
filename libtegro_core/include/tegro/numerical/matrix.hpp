#pragma once

#include <cstddef>
#include <vector>


namespace tegro {

class Matrix {
    public:
        Matrix(std::size_t rows, std::size_t columns);

        std::size_t rows() const;
        std::size_t columns() const;

        double& operator()(std::size_t rows, std::size_t columns);
        const double& operator()(std::size_t rows, std::size_t columns) const;

    private:
        std::size_t rows_;
        std::size_t columns_;

        std::vector<double> data_;
};

}