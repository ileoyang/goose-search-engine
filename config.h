#ifndef GOOSE_CONFIG_H
#define GOOSE_CONFIG_H

static const std::string DOCS_FILENAME = "msmarco-docs.trec";
static const std::string PARSE_FILE_PREFIX = "_pairs";
static const std::string MERGE_FILE_PREFIX = "pairs";
static const std::string TERMS_FILENAME = "terms";
static const std::string URLS_FILENAME = "urls";
static const std::string DOC_TERM_NUMS_FILENAME = "doc_term_nums";
static const std::string DOC_BEGINS_FILENAME = "doc_begins";
static const std::string INVERTED_INDEX_FILENAME = "inverted_index";
static const std::string LEXICON_VALUE_FILENAME = "lexicon_value";

#define LINEAR 1 << 0
#define LOGARITHMIC 1 << 1
#define FAGIN 1 << 2
#define RADIX 1 << 3
#define CONJUNCTIVE 1 << 4
#define DISJUNCTIVE 1 << 5

#define PARSE 1
#define MERGE 2
#define LIST 3
#define LOAD 4
#define QUERY 5
#define AUTOC 6
#define QUIT 7

#endif //GOOSE_CONFIG_H
