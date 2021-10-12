#ifndef GOOSE_DELTA_H
#define GOOSE_DELTA_H

#include <vector>

namespace delta {

void encode(const std::vector<int>& nums, std::vector<uint8_t>& bytes);

void decode(const std::vector<uint8_t>& bytes, std::vector<int>& nums);

}

#endif //GOOSE_DELTA_H
