#include "doc_info.h"

#include <fstream>

#include "../config.h"

std::vector<std::string> urls;
std::vector<int> doc_term_nums;
int doc_num;
double avg_doc_term_num;

void load_doc_info() {
    std::ifstream is(URLS_FILENAME);
    std::string url;
    while (is >> url) {
        urls.emplace_back(url);
    }
    doc_num = urls.size();
    doc_term_nums.resize(doc_num);
    is.close();
    is.open(DOC_TERM_NUMS_FILENAME, std::ios::binary);
    is.read(reinterpret_cast<char *>(&doc_term_nums[0]), doc_num * sizeof(int));
    size_t tot_doc_term_num = 0;
    for (int doc_term_num : doc_term_nums) {
        tot_doc_term_num += doc_term_num;
    }
    avg_doc_term_num = (double) tot_doc_term_num / doc_num;
}
