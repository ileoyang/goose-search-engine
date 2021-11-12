#include "lexicon.h"

#include <fstream>
#include <vector>

#include "../config.h"

std::unordered_map<std::string, entry> lexicon{};

void load_lexicon() {
    std::ifstream is(TERMS_FILENAME);
    std::string term;
    std::vector<std::string> terms;
    while (is >> term) {
        terms.emplace_back(term);
    }
    is.close();
    is.open(LEXICON_VALUE_FILENAME, std::ios::binary);
    size_t len = terms.size();
    std::vector<size_t> begins(len);
    std::vector<int> doc_nums(len);
    is.read(reinterpret_cast<char *>(&begins[0]), len * sizeof(size_t));
    is.read(reinterpret_cast<char *>(&doc_nums[0]), len * sizeof(int));
    for (int i = 0; i < len; i++) {
        lexicon[terms[i]] = entry{begins[i], doc_nums[i]};
    }
}
