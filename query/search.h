#ifndef GOOSE_SEARCH_H
#define GOOSE_SEARCH_H

#include <vector>
#include <string>

namespace goose_query {

void load(int option);

void conjunctive_search(const std::vector<std::string>& terms, int option = 0);

void disjunctive_search(const std::vector<std::string>& terms, int option = 0);

void search(const std::vector<std::string>& terms, int option);

void auto_complete(const std::string& term);

}

#endif //GOOSE_SEARCH_H
