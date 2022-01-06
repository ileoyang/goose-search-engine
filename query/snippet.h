#ifndef GOOSE_SNIPPET_H
#define GOOSE_SNIPPET_H

#include <string>
#include <vector>

namespace goose_query {

std::string get_snippet(int did, const std::vector<std::string>& terms);

}

#endif //GOOSE_SNIPPET_H
