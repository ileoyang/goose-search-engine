# Goose Search Engine

Goose is a search engine for indexing and querying TREC files.

## Architecture
<img width="612" src="https://user-images.githubusercontent.com/61306456/153310330-df38ac05-1f0b-48cb-ab7e-76ef23afd6ab.png">

## Feature
* I/O efficient circular buffer parse.
* External merge sort with binary heap.
* Index compression by variable byte and Elias delta coding.
* Snippet generation based on Aho-Corasick automaton.
* Top-k search pruning by Fagin's algorithm.
* Search autocomplete based on radix tree.
* Linear and logarithmic index quantization.

## Quick Start
```shell
# compile
mkdir build-dir && cd build-dir
cmake ..
make
# run
./goose
$ parse
$ merge
$ list
$ load
$ query -con Hello World
```
This project configures the default filename (`config.h`) to minimize command.

## Command
| Syntax | Description |
| ----------- | ----------- |
| `./goose` | Start Goose |
| `parse` | Parse TREC file into sorted postings |
| `merge` | Merge sorted postings |
| `list [OPTION]` | List sorted postings to inverted index |
| `load [OPTION]` | Load lexicon and document information |
| `query [OPTION] [TERM...]` | Query by terms |
| `autoc [TERM]` | Autocomplete by term |
| `quit` | Stop Goose |

## Option
| Syntax | Description | Related Command |
| ----------- | ----------- | ----------- |
| `-lin` | Linear quantization | `list`, `query` |
| `-log` | Logarithmic quantization | `list`, `query` |
| `-fag` | Fagin's algorithm | `query` |
| `-rad` | Radix tree | `load` |
| `-con` | Conjunctive query | `query` |
| `-dis` | Disjunctive query | `query` |
