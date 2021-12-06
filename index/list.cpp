#include "list.h"

#include <vector>
#include <fstream>
#include <cmath>

#include "../compress/varbyte.h"
#include "../compress/delta.h"
#include "../util/bm25.h"

namespace goose_index {

static const int BUFFER_SIZE = 1 << 30;
static const int BLOCK_SIZE = 64;
static const double EPSILON = 1e-6;
static const double LINEAR_MAX = 32.0034;
static const double LINEAR_MIN = 0.00156031;
static const double LINEAR_INTERVAL = LINEAR_MAX - LINEAR_MIN + EPSILON;
static const double LOGARITHMIC_MAX = log(LINEAR_MAX);
static const double LOGARITHMIC_MIN = log(LINEAR_MIN);
static const double LOGARITHMIC_INTERVAL = LOGARITHMIC_MAX - LOGARITHMIC_MIN + EPSILON;

void list(int option, std::string pairs_file_name) {
    std::vector<uint64_t> pairs(BUFFER_SIZE / sizeof(uint64_t));
    std::ifstream is(pairs_file_name, std::ios::binary);
    std::ofstream os(INVERTED_INDEX_FILENAME, std::ios::binary);
    std::vector<int> dids; // document IDs in one inverted index
    std::vector<int> freqs; // frequencies in one inverted index
    int prev_tid = 1;
    int prev_did = 1;
    int freq = 0;
    std::vector<uint8_t> blocks;
    std::vector<int> last_dids; // last document IDs in each block
    std::vector<size_t> com_diff_did_sizes; // size of compressed difference of document IDs in each block
    std::vector<size_t> com_freq_sizes; // size of compressed frequencies in each block
    size_t begin = 0;
    std::vector<size_t> begins; // begin indexes in file of each inverted index
    std::vector<int> doc_nums; // number of documents in each inverted index
    while (is) {
        is.read(reinterpret_cast<char *>(&pairs[0]), pairs.size() * sizeof(uint64_t));
        int pairs_len = is.gcount() / sizeof(int64_t);
        for (int i = 0; i < pairs_len; i++) {
            int tid = pairs[i] >> 32;
            int did = pairs[i] & 0xFFFFFFFF;
            if (tid != prev_tid || did != prev_did) {
                dids.emplace_back(prev_did);
                freqs.emplace_back(freq);
                prev_did = did;
                freq = 0;
            }
            if (tid != prev_tid) {
                for (int j = 0; j < dids.size(); j += BLOCK_SIZE) {
                    std::vector<int> block_diff_dids = {dids[j]}; // difference of document IDs in one block
                    int n = std::min((int) dids.size(), j + BLOCK_SIZE);
                    for (int k = j + 1; k < n; k++) {
                        block_diff_dids.emplace_back(dids[k] - dids[k - 1]);
                    }
                    last_dids.emplace_back(dids[n - 1]);
                    size_t size = blocks.size();
                    varbyte::encode(block_diff_dids, blocks);
                    com_diff_did_sizes.emplace_back(blocks.size() - size);
                    if (option) {
                        for (int k = j; k < n; k++) {
                            double score = bm25(dids[k], freqs[k], dids.size());
                            if (option & LINEAR) {
                                blocks.emplace_back(floor((score - LINEAR_MIN) / LINEAR_INTERVAL * (1 << 8)));
                            } else if (option & LOGARITHMIC) {
                                blocks.emplace_back(floor((log(score) - LOGARITHMIC_MIN) / LOGARITHMIC_INTERVAL * (1 << 8)));
                            }
                        }
                    } else {
                        size = blocks.size();
                        std::vector<int> block_freqs(freqs.begin() + j, freqs.begin() + n); // frequencies in one block
                        delta::encode(block_freqs, blocks);
                        com_freq_sizes.emplace_back(blocks.size() - size);
                    }
                }
                doc_nums.emplace_back(dids.size());
                dids.clear();
                freqs.clear();
                begins.emplace_back(begin);
                begin += last_dids.size() * sizeof(int) + (com_diff_did_sizes.size() + com_freq_sizes.size()) * sizeof(size_t) + blocks.size();
                prev_tid = tid;
                os.write(reinterpret_cast<const char *>(&last_dids[0]), last_dids.size() * sizeof(int));
                last_dids.clear();
                os.write(reinterpret_cast<const char *>(&com_diff_did_sizes[0]), com_diff_did_sizes.size() * sizeof(size_t));
                com_diff_did_sizes.clear();
                os.write(reinterpret_cast<const char *>(&com_freq_sizes[0]), com_freq_sizes.size() * sizeof(size_t));
                com_freq_sizes.clear();
                os.write(reinterpret_cast<const char *>(&blocks[0]), blocks.size());
                blocks.clear();
            }
            freq++;
        }
    }
    os.close();
    os.open(LEXICON_VALUE_FILENAME, std::ios::binary);
    os.write(reinterpret_cast<const char *>(&begins[0]), begins.size() * sizeof(size_t));
    os.write(reinterpret_cast<const char *>(&doc_nums[0]), doc_nums.size() * sizeof(int));
    std::remove(pairs_file_name.c_str());
}

}
