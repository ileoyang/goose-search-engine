#include "inverted_list.h"

#include <climits>

#include "lexicon.h"
#include "../config.h"
#include "../compress/varbyte.h"
#include "../compress/delta.h"

inverted_list::inverted_list() {}

inverted_list::inverted_list(std::string term, int option) {
    option &= (LINEAR | LOGARITHMIC);
    entry entry = lexicon[term];
    is.open(INVERTED_INDEX_FILENAME, std::ios::binary);
    is.seekg(entry.begin);
    block_num = (entry.doc_num + 63) / 64;
    meta.last_dids.resize(block_num);
    is.read(reinterpret_cast<char *>(&meta.last_dids[0]), block_num * sizeof(int));
    meta.com_diff_did_sizes.resize(block_num);
    is.read(reinterpret_cast<char *>(&meta.com_diff_did_sizes[0]), block_num * sizeof(size_t));
    if (!option) {
        meta.com_freq_sizes.resize(block_num);
        is.read(reinterpret_cast<char *>(&meta.com_freq_sizes[0]), block_num * sizeof(size_t));
    }
    list_idx = block_idx = 0;
    this->option = option;
}

int inverted_list::next_geq(int did) {
    int target_list_idx = std::lower_bound(meta.last_dids.begin(), meta.last_dids.end(), did) - meta.last_dids.begin();
    if (target_list_idx == block_num) { // input document ID greater than any document IDs in list
        return INT_MAX;
    }
    if (target_list_idx >= list_idx) {
        for (int i = list_idx; i < target_list_idx; i++) {
            int block_size = option ? 64 : meta.com_freq_sizes[i];
            is.seekg((meta.com_diff_did_sizes[i] + block_size) * sizeof(size_t), std::ios::cur);
        }
        com_diff_dids.resize(meta.com_diff_did_sizes[target_list_idx]);
        is.read(reinterpret_cast<char *>(&com_diff_dids[0]), meta.com_diff_did_sizes[target_list_idx]);
        dids.clear();
        varbyte::decode(com_diff_dids, dids);
        for (int i = 1; i < dids.size(); i++) {
            dids[i] += dids[i - 1];
        }
        if (option) {
            scores.resize(dids.size());
            is.read(reinterpret_cast<char *>(&scores[0]), dids.size());
        } else {
            com_freqs.resize(meta.com_freq_sizes[target_list_idx]);
            is.read(reinterpret_cast<char *>(&com_freqs[0]), meta.com_freq_sizes[target_list_idx]);
            freqs.clear();
            delta::decode(com_freqs, freqs);
        }
        list_idx = target_list_idx + 1;
        block_idx = 0;
    }
    block_idx = std::lower_bound(dids.begin(), dids.end(), did) - dids.begin();
    return block_idx == dids.size() ? INT_MAX : dids[block_idx];
}

int inverted_list::get_freq() {
    return freqs[block_idx];
}

uint8_t inverted_list::get_score() {
    return scores[block_idx];
}
