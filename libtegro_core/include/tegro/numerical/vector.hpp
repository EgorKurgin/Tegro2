#pragma once

#include <cstddef>
#include <vector>

namespace tegro {

class Vector {
    public:
        Vector() = default;
        Vector(std::size_t size);

        std::size_t size() const;

        double& operator[](std::size_t index);
        const double& operator[](std::size_t index) const;

    private:
        std::vector<double> data_;
};

}