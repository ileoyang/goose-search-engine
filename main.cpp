#include "index/parse.h"
#include "index/merge.h"
#include "index/list.h"
#include "query/search.h"

int main() {
    // goose_index::parse();
    // goose_index::merge();
    goose_query::load();
    goose_index::list(LINEAR);
    goose_query::conjunctive_search({"Leo", "Yang"}, LINEAR);
    goose_query::disjunctive_search({"Leo", "Yang", "Tech"}, LINEAR);
}
