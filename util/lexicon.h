#ifndef GOOSE_LEXICON_H
#define GOOSE_LEXICON_H

#include <string>
#include <unordered_map>

struct entry {
    size_t begin;
    int doc_num;
};

extern std::unordered_map<std::string, entry> lexicon;

void load_lexicon();

#endif //GOOSE_LEXICON_H
