#include "search.h"

#include <queue>
#include <iostream>

#include "../util/inverted_list.h"
#include "../util/lexicon.h"
#include "../util/bm25.h"
#include "../util/doc_info.h"
#include "snippet.h"

namespace goose_query {

const int RESULT_NUM = 10;

void load() {
    load_lexicon();
    load_doc_info();
}

void conjunctive_search(const std::vector<std::string>& terms) {
    int n = terms.size();
    std::vector<inverted_list> lists(n);
    for (int i = 0; i < n; i++) {
        if (!lexicon.count(terms[i])) {
            return;
        }
        lists[i] = inverted_list(terms[i]);
    }
    int did = 0;
    std::vector<int> dids;
    std::priority_queue<std::pair<double, int>> pq;
    while (did != INT_MAX) {
        did = lists[0].next_geq(did);
        int anchor_did = did;
        for (int i = 1; i < n; i++) {
            did = lists[i].next_geq(did);
        }
        if (anchor_did == did && did != INT_MAX) {
            std::vector<int> freqs(n), doc_nums(n);
            for (int i = 0; i < n; i++) {
                freqs[i] = lists[i].get_freq();
                doc_nums[i] = lexicon[terms[i]].doc_num;
            }
            pq.push({-bm25(did, freqs, doc_nums), did});
            if (pq.size() > RESULT_NUM) {
                pq.pop();
            }
            did++;
        }
    }
    while (pq.size()) {
        auto [_, did] = pq.top();
        pq.pop();
        std::cout << urls[did - 1] << std::endl << get_snippet(did, terms) << std::endl;
    }
}

void disjunctive_search(const std::vector<std::string>& terms) {
    std::vector<double> score(doc_num + 1);
    for (std::string term : terms) {
        if (!lexicon.count(term)) {
            continue;
        }
        inverted_list list = inverted_list(term);
        int did = list.next_geq(0);
        int doc_num = lexicon[term].doc_num;
        while (did != INT_MAX) {
            score[did] += bm25(did, {list.get_freq()}, {doc_num});
            did = list.next_geq(did + 1);
        }
    }
    std::priority_queue<std::pair<double, int>> pq;
    for (int i = 1; i <= doc_num; i++) {
        if (score[i]) {
            pq.push({-score[i], i});
        }
        if (pq.size() > RESULT_NUM) {
            pq.pop();
        }
    }
    while (pq.size()) {
        auto [_, did] = pq.top();
        pq.pop();
        std::cout << urls[did - 1] << std::endl << get_snippet(did, terms) << std::endl;
    }
}

}
