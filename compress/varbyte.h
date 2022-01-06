#ifndef GOOSE_VARBYTE_H
#define GOOSE_VARBYTE_H

#include <vector>
#include <stdint.h>

namespace varbyte {

void encode(const std::vector<int>& nums, std::vector<uint8_t>& bytes);

void decode(const std::vector<uint8_t>& bytes, std::vector<int>& nums);

}

#endif //GOOSE_VARBYTE_H
