#include "snippet.h"

#include <array>
#include <fstream>
#include <tuple>

#include "../config.h"
#include "../util/aho_corasick.h"

namespace goose_query {

const int SNIPPET_LEN = 300;

const std::string get_doc(int did) {
    std::array<size_t, 2> doc_interval;
    std::ifstream is(DOC_BEGINS_FILENAME, std::ios::binary);
    is.seekg((did - 1) * sizeof(size_t));
    is.read(reinterpret_cast<char *>(&doc_interval[0]), 2 * sizeof(size_t));
    doc_interval[1] -= doc_interval[0];
    is.close();
    is.open(DOCS_FILENAME);
    is.seekg(doc_interval[0], std::ios::beg);
    std::string doc(doc_interval[1], 0);
    is.read(&doc[0], doc_interval[1]);
    return doc;
}

std::string get_snippet(int did, const std::vector<std::string>& terms) {
    const std::string doc = get_doc(did);
    aho_corasick ac = aho_corasick();
    for (std::string term : terms) {
        ac.insert(term);
    }
    ac.build();
    std::vector<std::pair<int, int>> pair_idxs = ac.query(doc); // terms' indexes in document
    std::vector<std::tuple<int, int, int>> tuple_idxs;
    std::vector<int> paint(doc.size() + 1); // paint status of characters in document
    for (auto [tid, end] : pair_idxs) {
        int begin = end - terms[tid].size() + 1;
        if (!std::isalnum(doc[begin - 1]) && !std::isalnum(doc[end + 1])) { // filter out valid terms
            tuple_idxs.emplace_back(std::make_tuple(tid, begin, end));
            for (int j = begin; j <= end; j++) {
                paint[j] = 1;
            }
        }
    }
    std::vector<std::pair<int, int>> snippet_idxs; // snippet terms' indexes in document
    int n = terms.size();
    int tot_term_num = n;
    int try_term_num = n;
    int quota = 0; // total length of terms
    for (std::string term : terms) {
        quota += term.size();
    }
    while (tot_term_num && try_term_num) {
        int sel_term_num = 0; // number of selected distinct terms
        int min_len = SNIPPET_LEN; // minimum length of snippet substring in document
        int begin; // begin index of snippet substring in document
        int end; // end index of snippet substring in document
        std::vector<int> cnt(n); // count for each term
        int m = tuple_idxs.size();
        for (int i = 0, j = 0; i < m; i++) {
            while (j < m && sel_term_num < try_term_num) {
                if (!cnt[std::get<0>(tuple_idxs[j])]) {
                    sel_term_num++;
                    quota -= terms[std::get<0>(tuple_idxs[j])].size();
                }
                cnt[std::get<0>(tuple_idxs[j])]++;
                j++;
            }
            if (sel_term_num == try_term_num && std::get<2>(tuple_idxs[j - 1]) - std::get<1>(tuple_idxs[i]) + quota < min_len) {
                min_len = std::get<2>(tuple_idxs[j - 1]) - std::get<1>(tuple_idxs[i]) + quota;
                begin = i;
                end = j - 1;
            }
            if (!--cnt[std::get<0>(tuple_idxs[i])]) {
                sel_term_num--;
                quota += terms[std::get<0>(tuple_idxs[i])].size();
            }
        }
        if (min_len != SNIPPET_LEN) {
            snippet_idxs.emplace_back(std::make_pair(std::get<1>(tuple_idxs[begin]), std::get<2>(tuple_idxs[end])));
            int pos = 0;
            std::vector<bool> sel_terms(n);
            for (int i = begin; i <= end; i++) {
                sel_terms[std::get<0>(tuple_idxs[i])] = true;
            }
            for (auto term_idx : tuple_idxs) {
                if (!sel_terms[std::get<0>(term_idx)]) {
                    tuple_idxs[pos++] = term_idx;
                }
            }
            tuple_idxs.resize(pos);
            tot_term_num -= try_term_num;
        } else {
            try_term_num--;
        }
    }
    int fill_len = SNIPPET_LEN; // total length used to fill both sides in sub snippets
    for (auto [begin, end] : snippet_idxs) {
        fill_len -= end - begin + 1;
    }
    fill_len /= snippet_idxs.size();
    for (auto [begin, end] : snippet_idxs) {
        if (begin - fill_len < 0) {
            end += fill_len;
        } else {
            begin -= fill_len;
        }
        if (end + fill_len >= doc.size()) {
            begin -= fill_len;
        } else {
            end += fill_len;
        }
        for (int i = begin; i <= end; i++) {
            paint[i] += 2;
        }
    }
    std::string snippet;
    bool is_front = true;
    for (int i = 0; i < doc.size(); i++) {
        paint[i] *= paint[i] != 1;
        if (i && paint[i - 1] != paint[i]) {
            if ((!paint[i - 1] || !paint[i]) && snippet.back() != '.') {
                snippet += "..."; // add boundary
            }
            if (paint[i - 1] == 3 || paint[i] == 3) {
                snippet += is_front ? "\x1B[31m" : "\033[0m";
                is_front ^= 1;
            }
        }
        if (paint[i] >= 2) {
            snippet += doc[i] != '\n' ? doc[i] : ' ';
        }
    }
    return snippet;
}

}
