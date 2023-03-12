#include "search.h"

#include <queue>
#include <iostream>
#include <climits>
#include <algorithm>

#include "../util/inverted_list.h"
#include "../util/lexicon.h"
#include "../util/bm25.h"
#include "../util/doc_info.h"
#include "snippet.h"
#include "../config.h"

namespace goose_query {

const int RESULT_NUM = 10;

void load(int option) {
    load_lexicon(option);
    load_doc_info();
}

std::vector<std::pair<std::string, std::string>> format_res(std::priority_queue<std::pair<double, int>>& pq, const std::vector<std::string>& terms) {
    std::vector<std::pair<std::string, std::string>> results;
    while (pq.size()) {
        auto [_, did] = pq.top();
        pq.pop();
        std::string res = urls[did - 1] + "\n" + get_snippet(did, terms) + "\n";
        std::cout << res;
        results.emplace_back(std::make_pair(urls[did - 1], get_snippet(did, terms)));
    }
    return results;
}

std::vector<std::pair<std::string, std::string>> conjunctive_search(const std::vector<std::string>& terms, int option) {
    option &= (LINEAR | LOGARITHMIC | FAGIN);
    int n = terms.size();
    std::vector<inverted_list> lists(n);
    for (int i = 0; i < n; i++) {
        if (!lexicon.count(terms[i])) {
            return {};
        }
        lists[i] = inverted_list(terms[i], option);
    }
    std::priority_queue<std::pair<double, int>> pq;
    if (option & (LINEAR | FAGIN)) {
        std::vector<std::pair<int, int>> elements[n];
        std::unordered_map<int, int> score_map[n];
        int len = INT_MAX;
        for (int i = 0; i < n; i++) {
            // simulate sequential list and lookup table
            int did = lists[i].next_geq(0);
            while (did != INT_MAX) {
                elements[i].emplace_back(std::make_pair(did, lists[i].get_score()));
                score_map[i][did] = lists[i].get_score();
                did = lists[i].next_geq(did + 1);
            }
            len = std::min(len, (int) elements[i].size());
            std::sort(elements[i].begin(), elements[i].end(), [](const std::pair<int, int> o1, const std::pair<int, int> o2) {
                return o1.second > o2.second;
            });
        }
        std::unordered_map<int, std::pair<int, int>> ocr_map;
        int cnt = 0;
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < n; j++) {
                int did = elements[j][i].first;
                ocr_map[did].first |= (1 << j);
                ocr_map[did].second += score_map[j][did];
                if (ocr_map[did].first == (1 << n) - 1 && ++cnt == RESULT_NUM) {
                    break;
                }
            }
            if (cnt == RESULT_NUM) {
                break;
            }
        }
        for (auto [did, ocr] : ocr_map) {
            for (int i = 0; i < n; i++) {
                if (!(ocr.first & (1 << i)) && score_map[i].count(did)) {
                    ocr.first |= (1 << i);
                    ocr.second += score_map[i][did];
                }
            }
            if (ocr.first == (1 << n) - 1) {
                pq.emplace(std::make_pair(-ocr.second, did));
                if (pq.size() > RESULT_NUM) {
                    pq.pop();
                }
            }
        }
    } else {
        int did = 0;
        while (did != INT_MAX) {
            did = lists[0].next_geq(did);
            int anchor_did = did;
            for (int i = 1; i < n; i++) {
                did = lists[i].next_geq(did);
            }
            if (anchor_did == did && did != INT_MAX) {
                double score = 0;
                if (option) {
                    for (int i = 0; i < n; i++) {
                        score += lists[i].get_score();
                    }
                } else {
                    std::vector<int> freqs(n), doc_nums(n);
                    for (int i = 0; i < n; i++) {
                        freqs[i] = lists[i].get_freq();
                        doc_nums[i] = lexicon[terms[i]].doc_num;
                    }
                    score = bm25(did, freqs, doc_nums);
                }
                pq.push({-score, did});
                if (pq.size() > RESULT_NUM) {
                    pq.pop();
                }
                did++;
            }
        }
    }
    return format_res(pq, terms);
}

std::vector<std::pair<std::string, std::string>> disjunctive_search(const std::vector<std::string>& terms, int option) {
    option &= (LINEAR | LOGARITHMIC | FAGIN);
    std::vector<double> score(doc_num + 1);
    for (std::string term : terms) {
        if (!lexicon.count(term)) {
            continue;
        }
        inverted_list list = inverted_list(term, option);
        int did = list.next_geq(0);
        int doc_num = lexicon[term].doc_num;
        while (did != INT_MAX) {
            score[did] += option ? list.get_score() : bm25(did, list.get_freq(), doc_num);
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
    return format_res(pq, terms);
}

std::vector<std::pair<std::string, std::string>> search(const std::vector<std::string>& terms, int option) {
    if (option & CONJUNCTIVE) {
        return conjunctive_search(terms, option);
    } else if (option & DISJUNCTIVE) {
        return disjunctive_search(terms, option);
    }
}

void auto_complete(const std::string& term) {
    for (std::string _term : radix.prefix(term)) {
        std::cout << _term << std::endl;
    }
}

}
