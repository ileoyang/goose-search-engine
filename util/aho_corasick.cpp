#include "aho_corasick.h"

#include <queue>

aho_corasick::aho_corasick() {
    next.resize(1);
    fail.resize(1);
    end.resize(1);
    tid = 1;
}

int index(char ch) {
    if (std::isdigit(ch)) {
        return ch - '0';
    }
    if (std::islower(ch)) {
        return ch - 'a' + 10;
    }
    if (std::isupper(ch)) {
        return ch - 'A' + 36;
    }
    return 62;
}

void aho_corasick::insert(const std::string& term) {
    int u = 0;
    for (char ch : term) {
        int c = index(ch);
        if (!next[u][c]) {
            next[u][c] = next.size();
            next.emplace_back();
            fail.emplace_back();
            end.emplace_back();
        }
        u = next[u][c];
    }
    end[u] = tid++;
}

void aho_corasick::build() {
    std::queue<int> q;
    for (int i = 0; i < S; i++) {
        if (next[0][i]) {
            q.push(next[0][i]);
        }
    }
    while (q.size()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < S; i++) {
            if (next[u][i]) {
                fail[next[u][i]] = next[fail[u]][i];
                q.push(next[u][i]);
            } else {
                next[u][i] = next[fail[u]][i];
            }
        }
    }
}

std::vector<std::pair<int, int>> aho_corasick::query(const std::string& document) {
    std::vector<std::pair<int, int>> indexes;
    int u = 0;
    for (int i = 0; i < document.size(); i++) {
        int c = index(document[i]);
        u = next[u][c];
        for (int j = u; j && end[j]; j = fail[j]) {
            indexes.emplace_back(std::make_pair(end[j] - 1, i));
        }
    }
    return indexes;
}
