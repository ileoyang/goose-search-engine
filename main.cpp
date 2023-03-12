#include <unordered_map>
#include <iostream>
#include <restbed>
#include <nlohmann/json.hpp>

#include "index/parse.h"
#include "index/merge.h"
#include "index/list.h"
#include "query/search.h"

std::unordered_map<std::string, int> arg_map{{"lin", LINEAR}, {"log", LOGARITHMIC}, {"fag", FAGIN}, {"rad", RADIX}, {"con", CONJUNCTIVE}, {"dis", DISJUNCTIVE}, {"parse", PARSE}, {"merge", MERGE}, {"list", LIST}, {"load", LOAD}, {"query", QUERY}, {"autoc", AUTOC}, {"quit", QUIT}};

void get_method_handler(const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    const auto& term_str = request->get_query_parameter("terms");
    const auto& option_str = request->get_query_parameter("options");
    std::vector<std::string> terms;
    std::stringstream term_ss(term_str);
    std::string term;
    while (std::getline(term_ss, term, ',')) {
        terms.emplace_back(term);
    }
    int options = 0;
    std::stringstream option_ss(option_str);
    std::string option;
    while (std::getline(option_ss, option, ',')) {
        options |= arg_map[option];
    }
    std::vector<std::pair<std::string, std::string>> results = goose_query::search(terms, options);
    nlohmann::json serialized_data;
    for (const auto& result : results) {
        serialized_data.push_back({{"url", result.first}, {"snippet", result.second}});
    }
    session->close(restbed::OK, serialized_data.dump(), {{"Content-Type", "application/json"}, { "Connection", "close" }});
}

void service_ready_handler(restbed::Service&) {
    fprintf(stderr, "The API service is up and running.");
}

int main(int argc, char* argv[]) {
    if (argc == 2 && strcmp(argv[1], "-api") == 0) {
        goose_query::load(0);
        auto resource = std::make_shared<restbed::Resource>();
        resource->set_path("/api/search");
        resource->set_method_handler("GET", get_method_handler);
        auto settings = std::make_shared<restbed::Settings>();
        settings->set_port(5000);
        auto service = std::make_shared<restbed::Service>();
        service->publish(resource);
        service->set_ready_handler(service_ready_handler);
        service->start(settings);
    }
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
