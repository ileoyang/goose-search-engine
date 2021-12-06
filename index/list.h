#ifndef GOOSE_LIST_H
#define GOOSE_LIST_H

#include <string>

#include "../config.h"

namespace goose_index {

void list(int option = 0, std::string pairs_file_name = MERGE_FILE_PREFIX + "0");

}

#endif //GOOSE_LIST_H
