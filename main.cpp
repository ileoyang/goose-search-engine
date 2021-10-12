#include "index/parse.h"
#include "index/merge.h"
#include "index/list.h"

int main() {
    goose_index::parse();
    goose_index::merge();
    goose_index::list();
}
