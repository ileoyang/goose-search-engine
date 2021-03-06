#ifndef GOOSE_BM25_H
#define GOOSE_BM25_H

#include <vector>

double bm25(int did, const std::vector<int>& freqs, const std::vector<int>& doc_nums);

double bm25(int did, int freq, int doc_num);

#endif //GOOSE_BM25_H
