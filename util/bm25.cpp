#include "bm25.h"

#include <cmath>

#include "doc_info.h"

double k1 = 1.2;
double b = 0.75;

double get_k(int did) {
    return k1 * (1 - b + b * doc_term_nums[did - 1] / avg_doc_term_num);
}

double get_score(int freq, int doc_num, double k) {
    double idf = log((::doc_num - doc_num + 0.5) / (doc_num + 0.5) + 1);
    return idf * freq * (k1 + 1) / (freq + k);
}

double bm25(int did, const std::vector<int>& freqs, const std::vector<int>& doc_nums) {
    double k = get_k(did);
    double score = 0;
    for (int i = 0; i < freqs.size(); i++) {
        score += get_score(freqs[i], doc_nums[i], k);
    }
    return score;
}

double bm25(int did, int freq, int doc_num) {
    double k = get_k(did);
    return get_score(freq, doc_num, k);
}
