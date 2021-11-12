#ifndef GOOSE_DOC_INFO_H
#define GOOSE_DOC_INFO_H

#include <vector>
#include <string>

extern std::vector<std::string> urls;
extern std::vector<int> doc_term_nums; // number of terms in each document
extern int doc_num; // number of documents
extern double avg_doc_term_num; // average number of terms in documents

void load_doc_info();

#endif //GOOSE_DOC_INFO_H
