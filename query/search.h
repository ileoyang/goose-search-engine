#ifndef GOOSE_SEARCH_H
#define GOOSE_SEARCH_H

#include <vector>
#include <string>

namespace goose_query {

void load(int option);

std::vector<std::pair<std::string, std::string>> conjunctive_search(const std::vector<std::string>& terms, int option = 0);

std::vector<std::pair<std::string, std::string>> disjunctive_search(const std::vector<std::string>& terms, int option = 0);

std::vector<std::pair<std::string, std::string>> search(const std::vector<std::string>& terms, int option);

void auto_complete(const std::string& term);

}

#endif //GOOSE_SEARCH_H
