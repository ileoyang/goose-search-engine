#include <unordered_map>
#include <iostream>
#include <sstream>

#include "index/parse.h"
#include "index/merge.h"
#include "index/list.h"
#include "query/search.h"

int main(int argc, char* argv[]) {
    std::cout << R"(
    ╔═╗┌─┐┌─┐┌─┐┌─┐
    ║ ╦│ ││ │└─┐├┤
    ╚═╝└─┘└─┘└─┘└─┘
    )" << std::endl;
    std::unordered_map<std::string, int> arg_map{{"lin", LINEAR}, {"log", LOGARITHMIC}, {"fag", FAGIN}, {"rad", RADIX}, {"con", CONJUNCTIVE}, {"dis", DISJUNCTIVE}, {"parse", PARSE}, {"merge", MERGE}, {"list", LIST}, {"load", LOAD}, {"query", QUERY}, {"autoc", AUTOC}, {"quit", QUIT}};
    if (argc == 2) {
        switch (arg_map[argv[1]]) {
            case PARSE:
                goose_index::parse();
                break;
            case MERGE:
                goose_index::merge();
                break;
            case LIST:
                goose_index::list();
                break;
        }
    } else while (1) {
        std::cout << "$ ";
        std::string line;
        std::getline(std::cin, line);
        std::istringstream args(line);
        std::string arg;
        std::string command;
        int option = 0;
        std::vector<std::string> terms;
        args >> command;
        while (args >> arg) {
            if (arg[0] == '-') {
                option |= arg_map[arg.substr(1)];
            } else {
                terms.emplace_back(arg);
            }
        }
        switch (arg_map[command]) {
            case PARSE:
                goose_index::parse();
                break;
            case MERGE:
                goose_index::merge();
                break;
            case LIST:
                goose_index::list(option);
                break;
            case LOAD:
                goose_query::load(option);
                break;
            case QUERY:
                goose_query::search(terms, option);
                break;
            case AUTOC:
                goose_query::auto_complete(terms.front());
                break;
            case QUIT:
                return 0;
        }
    }
}
