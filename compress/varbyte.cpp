#include "varbyte.h"

namespace varbyte {

void encode(const std::vector<int>& nums, std::vector<uint8_t>& bytes) {
    for (int num : nums) {
        while (num > 127) {
            bytes.emplace_back(num & 127);
            num >>= 7;
        }
        bytes.emplace_back(num + 128);
    }
}

void decode(const std::vector<uint8_t>& bytes, std::vector<int>& nums) {
    int num = 0, idx = 0;
    for (uint8_t byte : bytes) {
        if (byte >= 128) {
            num += (byte & 127) << idx;
            nums.emplace_back(num);
            num = idx = 0;
        } else {
            num += byte << idx;
            idx += 7;
        }
    }
}

}
