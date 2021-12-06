#ifndef GOOSE_SEARCH_H
#define GOOSE_SEARCH_H

#include <vector>
#include <string>

namespace goose_query {

void load();

void conjunctive_search(const std::vector<std::string>& terms, int option = 0);

void disjunctive_search(const std::vector<std::string>& terms, int option = 0);

}

#endif //GOOSE_SEARCH_H
