#include <tegro/numerical/vector.hpp>

namespace tegro {

Vector::Vector(std::size_t size)
    : data_(size) {
}

std::size_t Vector::size() const {
    return data_.size();
}

double& Vector::operator[](std::size_t index) {
    return data_[index];
}

const double& Vector::operator[](std::size_t index) const {
    return data_[index];
}

}