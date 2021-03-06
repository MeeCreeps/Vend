//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_BFILTER_INT_ENCODE_H
#define VEND_BFILTER_INT_ENCODE_H

#include "bfilter_encode.h"
#include <cstring>

class BFilterIntEncode : public BFilterEncode {

public:
    BFilterIntEncode() : BFilterEncode(INT_HASH_BEST_NUMS, VERTEX_SIZE * K_SIZE) {
        memset(encode_, 0, VERTEX_SIZE * K_SIZE * sizeof(uint32_t));
    }

    PairType NEpairTest(uint32_t vertex1, uint32_t vertex2) override;

    void EdgeSet(uint32_t vertex1, uint32_t vertex2) override;

    void InsertPair(uint32_t vertex1, uint32_t vertex2) override;

    void DeletePair(uint32_t vertex1, uint32_t vertex2) override;

    void LoadFromDb(std::string file_path) override;


    void EncodePersistent(std::string file_path) override;

protected:
    uint32_t encode_[VERTEX_SIZE * K_SIZE];

};

#endif //VEND_BFILTER_INT_ENCODE_H
