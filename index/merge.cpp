#include "merge.h"

#include <vector>
#include <fstream>

namespace goose_index {

#define head(x) ipairs[x][ipairs_idx[x]]

static const int BUFFER_SIZE = 1 << 30;

struct bin_heap {
    std::vector<int> fids;
    int n;
    std::vector<std::vector<uint64_t>>& ipairs;
    std::vector<int>& ipairs_idx;

    bin_heap(std::vector<std::vector<uint64_t>>& ipairs, std::vector<int>& ipairs_idx) : ipairs(ipairs), ipairs_idx(ipairs_idx) {
        n = ipairs.size();
        fids.resize(n + 1);
        for (int i = 0; i < n; i++) {
            fids[i + 1] = i;
        }
        for (int i = n >> 1; i; i--) {
            down(i);
        }
    }

    uint64_t key(int x) {
        return head(fids[x]);
    }

    void down(int x) {
        int t;
        while ((t = x << 1) <= n) {
            if (t + 1 <= n && key(t) > key(t + 1)) {
                t++;
            }
            if (key(x) < key(t)) {
                break;
            }
            fids[x] ^= fids[t];
            fids[t] ^= fids[x];
            fids[x] ^= fids[t];
            x = t;
        }
    }

    void swap() {
        std::swap(fids[1], fids[n--]);
    }

    int top() {
        return fids[1];
    }

    bool not_empty() {
        return n;
    }
};

void merge(std::string ipairs_file_prefix, int ipairs_file_num, std::string opairs_file_prefix, int opairs_file_num) {
    int max_degree = (ipairs_file_num + opairs_file_num - 1) / opairs_file_num;
    for (int i = 0; i < ipairs_file_num; i += max_degree) {
        int degree = std::min(max_degree, ipairs_file_num - i); // number of files to merge
        int chunk_len = BUFFER_SIZE / (degree + 1) / sizeof(uint64_t);
        std::vector<std::vector<uint64_t>> ipairs(degree, std::vector<uint64_t>(chunk_len));
        std::vector<int> ipairs_idx(degree);
        std::vector<int> size(degree);
        std::vector<uint64_t> opairs(chunk_len);
        int opairs_idx = 0;
        std::ifstream is[degree];
        for (int j = 0; j < degree; j++) {
            is[j] = std::ifstream(ipairs_file_prefix + std::to_string(i + j), std::ios::binary);
            is[j].read(reinterpret_cast<char *>(&ipairs[j][0]), chunk_len * sizeof(uint64_t));
            size[j] = is[j].gcount() / sizeof(uint64_t);
        }
        std::ofstream os(opairs_file_prefix + std::to_string(i / max_degree), std::ios::binary);
        bin_heap heap(ipairs, ipairs_idx);
        while (heap.not_empty()) {
            int fid = heap.top();
            opairs[opairs_idx] = head(fid);
            if (++opairs_idx == chunk_len) {
                os.write(reinterpret_cast<const char *>(&opairs[0]), chunk_len * sizeof(uint64_t));
                opairs_idx = 0;
            }
            if (++ipairs_idx[fid] == size[fid]) {
                is[fid].read(reinterpret_cast<char *>(&ipairs[fid][0]), chunk_len * sizeof(uint64_t));
                size[fid] = is[fid].gcount() / sizeof(uint64_t);
                if (!size[fid]) {
                    heap.swap();
                }
                ipairs_idx[fid] = 0;
            }
            heap.down(1);
        }
        if (opairs_idx) {
            os.write(reinterpret_cast<const char *>(&opairs[0]), opairs_idx * sizeof(uint64_t));
        }
        for (int j = 0; j < degree; j++) {
            std::remove((ipairs_file_prefix + std::to_string(i + j)).c_str());
        }
    }
}

}
