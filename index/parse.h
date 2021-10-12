#ifndef GOOSE_PARSE_H
#define GOOSE_PARSE_H

#include <string>

#include "../config.h"

namespace goose_index {

void parse(std::string docs_filename = DOCS_FILENAME, std::string parse_file_prefix = PARSE_FILE_PREFIX);

}

#endif //GOOSE_PARSE_H
