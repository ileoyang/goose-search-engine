#include "index/parse.h"
#include "index/merge.h"
#include "index/list.h"
#include "query/search.h"

int main() {
//    goose_index::parse();
//    goose_index::merge();
//    goose_index::list();
    goose_query::load();
    goose_query::conjunctive_search({"Leo", "Yang"});
    goose_query::disjunctive_search({"Leo", "Yang", "Tech"});
}
