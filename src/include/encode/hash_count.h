
#ifndef VEND_HASH_COUNT_H
#define VEND_HASH_COUNT_H

#include "common/config.h"
#include <vector>
#include <cstring>

static uint32_t PREFIX_BIT_SIZE = LOG_K + 3;
enum class BlockType {
    FullHash, LeftMost, RightMost, Middle
};

// class fro dynamic choose for hybrid encode
class HashCount {
public:
    HashCount() {
        memset(hash_array_, 0, sizeof(uint32_t) * PER_ENCODE_BIT_SIZE);
        memset(pre_nepair_, 0, sizeof(uint32_t) * PER_ENCODE_BIT_SIZE);
    }

    HashCount(const std::vector<uint32_t> &neighbors, const uint32_t &block_size) {
        ReConstruct(neighbors, block_size);
    }

    inline uint32_t Hash(const uint32_t &key) {
        return hash_size_ == 0 ? 0 : key % hash_size_;
    }

    void SetHashSize(const uint32_t &hash_size) {
        hash_size_ = hash_size;
    }

    uint32_t RemoveHash(const uint32_t &key);

    uint32_t AddHash(const uint32_t &key);

    // when block size +1, call the function to reconstruct pre_nepair and hash_array
    void ReConstruct(const std::vector<uint32_t> &neighbors, const uint32_t &block_size, bool reverse = false);

    uint32_t GetScore(const std::vector<uint32_t> &neighbors, const uint32_t &block_size, const uint32_t &first_idx,
                      const BlockType &type);

protected:
    uint32_t hash_array_[PER_ENCODE_BIT_SIZE];
    // pre_nepair[i] : from 0~i, how many 0 bits
    uint32_t pre_nepair_[PER_ENCODE_BIT_SIZE];
    uint32_t hash_size_;
    uint32_t neighbor_size_;


};


#endif //VEND_HASH_COUNT_H
