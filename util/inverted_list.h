#ifndef GOOSE_INVERTED_LIST_H
#define GOOSE_INVERTED_LIST_H

#include <vector>
#include <string>
#include <fstream>

struct metadata {
    std::vector<int> last_dids;
    std::vector<size_t> com_diff_did_sizes;
    std::vector<size_t> com_freq_sizes;
};

class inverted_list {
private:
    std::ifstream is;
    metadata meta;
    int block_num;
    int list_idx;
    int block_idx;
    int option;
    std::vector<int> dids;
    std::vector<int> freqs;
    std::vector<uint8_t> scores;
    std::vector<uint8_t> com_diff_dids;
    std::vector<uint8_t> com_freqs;
public:
    inverted_list();
    inverted_list(std::string term, int option = 0);
    int next_geq(int did);
    int get_freq();
    uint8_t get_score();
};

#endif //GOOSE_INVERTED_LIST_H
