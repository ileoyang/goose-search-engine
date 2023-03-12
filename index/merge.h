#ifndef GOOSE_MERGE_H
#define GOOSE_MERGE_H

#include <string>

#include "../config.h"

namespace goose_index {

void merge(std::string ipairs_file_prefix = PARSE_FILE_PREFIX, std::string opairs_file_prefix = MERGE_FILE_PREFIX, int opairs_file_num = 1);

}

#endif //GOOSE_MERGE_H
