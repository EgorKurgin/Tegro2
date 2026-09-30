#pragma once

#include <string>
#include <vector>
#include <utility>
#include <memory>

#include <tegro/reactions/rate_constant.hpp>

namespace tegro {

struct MonoRecord {
    std::string src;
    std::string dst;
    const MonomolecularRate* rate;
};

class SystemBuilder {
    public:
        void add_mono(std::string src, std::string dst,
                      const MonomolecularRate* rate) {
            mono_.push_back({std::move(src), std::move(dst), rate});
        }

        const std::vector<MonoRecord>& mono() const noexcept {
            return mono_;
        }
    private:
        std::vector<MonoRecord> mono_;
};

}