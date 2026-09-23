#include <cassert>

#include <tegro/numerical/vector.hpp>

int main() {
    tegro::Vector vector(10);

    assert(vector.size() == 10);

    vector[3] = 42.0;
    
    assert(vector[3] == 42.0);

    return 0;
};