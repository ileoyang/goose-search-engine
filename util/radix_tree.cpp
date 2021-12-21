#include "radix_tree.h"

#include "aho_corasick.h"

node::node(std::string edge) {
    this->edge = edge;
    is_end = false;
}

node::~node() {}

radix_tree::radix_tree() {
    root = std::make_shared<node>("");
}

void radix_tree::insert(const std::string& term) {
    auto p = root;
    for (int i = 0, j; i < term.size(); i += j) {
        int u = index(term[i]);
        if (!p->son[u]) {
            p = p->son[u] = std::make_shared<node>(term.substr(i));
            break;
        }
        std::string edge = p->son[u]->edge;
        j = 0;
        while (j < edge.size() && i + j < term.size() && edge[j] == term[i + j]) {
            j++;
        }
        if (j < edge.size()) {
            auto s = p->son[u];
            s->edge = edge.substr(j);
            p->son[u] = std::make_shared<node>(edge.substr(0, j));
            p->son[u]->son[index(s->edge[0])] = s;
        }
        p = p->son[u];
    }
    p->is_end = true;
}

bool radix_tree::find(const std::string& term) {
    auto p = root;
    for (int i = 0, j; i < term.size(); i += j) {
        int u = index(term[i]);
        if (!p->son[u]) {
            return false;
        }
        std::string edge = p->son[u]->edge;
        if (edge.size() > term.size() - i) {
            return false;
        }
        for (j = 0; j < edge.size(); j++) {
            if (edge[j] != term[i + j]) {
                return false;
            }
        }
        p = p->son[u];
    }
    return p->is_end;
}

void get_leaves(std::shared_ptr<node> p, std::vector<std::string>& terms, std::string path, int k) {
    if (terms.size() == k) {
        return;
    }
    path += p->edge;
    if (p->is_end) {
        terms.emplace_back(path);
    }
    for (auto [_, s] : p->son) {
        get_leaves(s, terms, path, k);
    }
}

std::vector<std::string> radix_tree::prefix(const std::string& term) {
    auto p = root;
    for (int i = 0, j; i < term.size(); i += j) {
        int u = index(term[i]);
        if (!p->son[u]) {
            return {};
        }
        std::string edge = p->son[u]->edge;
        j = 0;
        while (j < edge.size() && i + j < term.size() && edge[j] == term[i + j]) {
            j++;
        }
        if (i + j == term.size()) {
            std::vector<std::string> terms;
            std::string path = term.substr(0, i);
            get_leaves(p->son[u], terms, path, 10);
            return terms;
        }
        if (j != edge.size()) {
            return {};
        }
        p = p->son[u];
    }
}
