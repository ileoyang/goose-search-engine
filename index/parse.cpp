#include "parse.h"

#include <vector>
#include <unordered_map>
#include <fstream>

namespace goose_index {

static const int IBUFFER_SIZE = 1 << 29;
static const int TERM_MAX_LEN = 20;

void parse(std::string docs_filename, std::string pairs_file_prefix) {
    std::string docs(IBUFFER_SIZE, 0);
    int docs_len = 0; // active length of documents buffer
    std::string term;
    std::string terms; // a bunch of terms seperated by spaces
    std::ofstream os_terms(TERMS_FILENAME);
    bool term_too_long = false;
    int tid = 1; // term ID
    int did = 1; // document ID
    std::unordered_map<std::string, int> term_map({{"TEXT", -1}}); // map from term to its ID
    std::vector<uint64_t> pairs; // pairs of term ID and document ID
    int pairs_fid = 0; // suffix id of parsed pairs file
    int skip_quota = 5; // line skip quota for boundary of documents
    std::string urls;
    std::ofstream os_urls(URLS_FILENAME);
    int doc_term_num = 0; // number of terms in each document
    std::vector<int> doc_term_nums;
    std::ofstream os_doc_term_nums(DOC_TERM_NUMS_FILENAME);
    size_t docs_idx = 0; // current access index in documents file
    std::vector<size_t> doc_begins; // begin index in file of each document
    std::ofstream os_doc_begins(DOC_BEGINS_FILENAME);
    std::ifstream is(docs_filename);
    while (is) {
        is.read(&docs[docs_len], docs.size() - docs_len);
        docs_len += is.gcount();
        // make active input buffer integral
        while (docs[--docs_len] != '\n');
        docs_len -= (docs.substr(docs_len - 6, 6) == "<TEXT>") * 6;
        for (int i = 0; i <= docs_len; i++) {
            if (std::isalnum(docs[i])) {
                if (!term_too_long) {
                    term += docs[i];
                    if (term.size() > TERM_MAX_LEN) {
                        term_too_long = true;
                        term.clear();
                    }
                }
            } else { // delimiter
                if (term.size()) {
                    if (skip_quota) {
                        if (!--skip_quota) { // begin of document
                            std::string url = "\n";
                            i += 2;
                            while (docs[i] != '\n') {
                                url += docs[i++];
                            }
                            urls += url;
                            doc_begins.emplace_back(docs_idx + i);
                        }
                    } else {
                        uint64_t val = term_map[term];
                        if (val == -1 && i >= 6 && docs[i - 6] == '<' && docs[i - 5] == '/' && docs[i] == '>') { // end of document
                            skip_quota = 6;
                            did++;
                            doc_term_nums.emplace_back(doc_term_num);
                            doc_term_num = 0;
                        } else { // new term
                            if (!val) { // unvisited
                                term_map[term] = val = tid++;
                                terms += term + " ";
                            }
                            pairs.emplace_back((val << 32) + did);
                            doc_term_num++;
                        }
                    }
                    term.clear();
                }
                term_too_long = false;
            }
        }
        docs_idx += docs_len;
        int next_len = 0;
        while (next_len + docs_len < docs.size()) {
            docs[next_len] = docs[next_len + docs_len];
            next_len++;
        }
        docs_len = next_len;
        std::sort(pairs.begin(), pairs.end());
        std::ofstream os(pairs_file_prefix + std::to_string(pairs_fid++), std::ios::binary);
        os.write(reinterpret_cast<const char*>(&pairs[0]), pairs.size() * sizeof(uint64_t));
        pairs.clear();
        os_terms.write(&terms[0], terms.size());
        terms.clear();
        os_urls.write(&urls[0], urls.size());
        urls.clear();
        os_doc_term_nums.write(reinterpret_cast<const char *>(&doc_term_nums[0]), doc_term_nums.size() * sizeof(int));
        doc_term_nums.clear();
        os_doc_begins.write(reinterpret_cast<const char *>(&doc_begins[0]), doc_begins.size() * sizeof(size_t));
        doc_begins.clear();
    }
}

}
