#include "delta.h"

#include <cmath>

namespace delta {

bool get_bit(const std::vector<uint8_t>& bytes, int idx) {
    return (idx >> 3) >= bytes.size() ? 0 : bytes[idx >> 3] & (1 << (idx & 7));
}

void set_bit(std::vector<uint8_t>& bytes, int idx) {
    while ((idx >> 3) >= bytes.size()) {
        bytes.emplace_back(0);
    }
    bytes[idx >> 3] |= (1 << (idx & 7));
}

void encode(const std::vector<int>& nums, std::vector<uint8_t>& bytes) {
    int idx = bytes.size() << 3;
    for (int num : nums) {
        int len = 1 + floor(log2(num));
        int lol = floor(log2(len));
        idx += lol;
        for (int i = lol; i >= 0; i--) {
            if ((len >> i) & 1) {
                set_bit(bytes, idx);
            }
            idx++;
        }
        for (int i = len - 2; i >= 0; i--) {
            if ((num >> i) & 1) {
                set_bit(bytes, idx);
            }
            idx++;
        }
        while (idx > (bytes.size() << 3)) {
            bytes.emplace_back(0);
        }
    }
}

void decode(const std::vector<uint8_t>& bytes, std::vector<int>& nums) {
    int idx = 0;
    while (idx < (bytes.size() << 3)) {
        int num = 1;
        int len = 1;
        int lol = 0;
        while (idx < bytes.size() << 3 && !get_bit(bytes, idx++)) {
            lol++;
        }
        for (int i = 0; i < lol; i++) {
            len <<= 1;
            if (get_bit(bytes, idx++)) {
                len |= 1;
            }
        }
        for (int i = 0; i < len - 1; i++) {
            num <<= 1;
            if (get_bit(bytes, idx++)) {
                num |= 1;
            }
        }
        if (idx <= (bytes.size() << 3)) {
            nums.emplace_back(num);
        }
    }
}

}
