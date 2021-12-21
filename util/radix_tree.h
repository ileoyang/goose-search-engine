#ifndef GOOSE_RADIX_TREE_H
#define GOOSE_RADIX_TREE_H

#include <unordered_map>
#include <string>

struct node {
    std::unordered_map<char, std::shared_ptr<node>> son;
    std::string edge;
    bool is_end;
    node(std::string edge);
    virtual ~node();
};

class radix_tree {
private:
    std::shared_ptr<node> root;
public:
    radix_tree();
    void insert(const std::string& term);
    bool find(const std::string& term);
    std::vector<std::string> prefix(const std::string& term);
};

#endif //GOOSE_RADIX_TREE_H
