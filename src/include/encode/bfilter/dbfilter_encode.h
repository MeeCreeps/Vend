

#ifndef VEND_DBFILTER_ENCODE_H
#define VEND_DBFILTER_ENCODE_H

#include "bfilter_bit_encode.h"

class DBFilterEncode : public BFilterBitEncode {

public:
    DBFilterEncode() : BFilterBitEncode(), block_len_(BLOCK_SIZE), left_size_(LEFT_SIZE), region_size_(REGION_SIZE) {};

    PairType NEpairTest(uint32_t vertex1, uint32_t vertex2) override;

    void EdgeSet(uint32_t vertex1, uint32_t vertex2) override;

    void InsertPair(uint32_t vertex1, uint32_t vertex2) override;

    uint64_t Hash(uint32_t key1, uint32_t key2, uint32_t hash_param1, uint32_t hash_param2) override {
        return ((uint64_t) key1 * hash_param1 + (uint64_t) key2 * hash_param2) % left_size_;
    }

     bool IsDeletable(uint32_t vertex1, uint32_t vertex2) override;

    void DeletePair(uint32_t vertex1, uint32_t vertex2) override;
protected:
    uint32_t block_len_;

    uint32_t region_size_;
    //  is equal to  hash_size_ - regin_size_
    uint64_t left_size_;
    // how many bits one block take ,default:512bit


};

#endif //VEND_DBFILTER_ENCODE_H
