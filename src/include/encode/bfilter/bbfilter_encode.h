
#ifndef VEND_BBFILTER_ENCODE_H
#define VEND_BBFILTER_ENCODE_H


#include "bfilter_bit_encode.h"

class BBFilterEncode : public BFilterBitEncode {

public:
    BBFilterEncode()
            : BFilterBitEncode(), block_size_(BLOCK_SIZE),
              block_nums_(block_nums_) {
//            best_hash_nums_=1;
    }


    PairType NEpairTest(uint32_t vertex1, uint32_t vertex2) override {

        if (vertex1 > vertex2)
            std::swap(vertex1, vertex2);
        uint32_t block_offset = BlockHash(vertex1, vertex2) * block_size_;
        for (int i = 0; i < best_hash_nums_; ++i) {
            if (!encode_.IsOne(block_offset + Hash(vertex1, vertex2, hash_param1_[i], hash_param2_[i])))
                return PairType::NonNeighbor;
        }
        return PairType::Uncertain;

    };

    void EdgeSet(uint32_t vertex1, uint32_t vertex2) override {
        if (vertex1 > vertex2)
            std::swap(vertex1, vertex2);

        uint32_t block_offset = BlockHash(vertex1, vertex2) * block_size_;
        for (int i = 0; i < best_hash_nums_; ++i)
            encode_.SetOne(block_offset + Hash(vertex1, vertex2, hash_param1_[i], hash_param2_[i]));


    };

    void InsertPair(uint32_t vertex1, uint32_t vertex2) override { EdgeSet(vertex1, vertex2); };

    // return block index
    uint32_t BlockHash(uint32_t key1, uint32_t key2) {
        return ((uint64_t) key1 * HASH_PARAM1[0] + (uint64_t) key2 * HASH_PARAM2[0]) % block_nums_;
    }

    uint64_t Hash(uint32_t key1, uint32_t key2, uint32_t hash_param1, uint32_t hash_param2) override {
        return ((uint64_t) key1 * hash_param1 + (uint64_t) key2 * hash_param2) % block_size_;
    }


protected:
    // per bits for counting
    uint32_t block_size_;
    // how many elements for the whole bitset
    uint64_t block_nums_;


};


#endif //VEND_BBFILTER_ENCODE_H
