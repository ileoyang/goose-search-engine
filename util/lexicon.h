#ifndef GOOSE_LEXICON_H
#define GOOSE_LEXICON_H

#include <string>
#include <unordered_map>

#include "radix_tree.h"

struct entry {
    size_t begin;
    int doc_num;
};

extern std::unordered_map<std::string, entry> lexicon;
extern radix_tree radix;

void load_lexicon(int option);

#endif //GOOSE_LEXICON_H
