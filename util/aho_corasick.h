#ifndef GOOSE_AHO_CORASICK_H
#define GOOSE_AHO_CORASICK_H

#include <vector>
#include <array>
#include <string>

class aho_corasick {
private:
    static const int S = 62;
    std::vector<std::array<int, S + 1>> next;
    std::vector<int> fail;
    std::vector<int> end;
    int tid;
public:
    aho_corasick();
    void insert(const std::string& term);
    void build();
    std::vector<std::pair<int, int>> query(const std::string& document);
};

int index(char ch);

#endif //GOOSE_AHO_CORASICK_H
