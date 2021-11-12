#include "bm25.h"

#include <cmath>

#include "doc_info.h"

double bm25(int did, const std::vector<int>& freqs, const std::vector<int>& doc_nums) {
    double k1 = 1.2, b = 0.75;
    double k = k1 * (1 - b + b * doc_term_nums[did - 1] / avg_doc_term_num);
    double score = 0;
    for (int i = 0; i < freqs.size(); i++) {
        double idf = log((doc_num - doc_nums[i] + 0.5) / (doc_nums[i] + 0.5) + 1);
        score += idf * freqs[i] * (k1 + 1) / (freqs[i] + k);
    }
    return score;
}
