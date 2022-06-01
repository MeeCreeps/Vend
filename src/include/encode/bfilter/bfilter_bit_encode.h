//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_BFILTER_BIT_ENCODE_H
#define VEND_BFILTER_BIT_ENCODE_H

#include "util/bitset.h"
#include "bfilter_encode.h"
#include <algorithm>

class BFilterBitEncode : public BFilterEncode {

public:
    BFilterBitEncode() : BFilterEncode(BIT_HASH_BEST_NUMS, (uint64_t)VERTEX_SIZE*K_SIZE  * 32) {}

    PairType NEpairTest(uint32_t vertex1, uint32_t vertex2) override;

    void EdgeSet(uint32_t vertex1, uint32_t vertex2) override;

    void InsertPair(uint32_t vertex1, uint32_t vertex2) override;

    void Clear() override { encode_.Clear(); };

    void LoadFromDb(std::string file_path) override;


    void EncodePersistent(std::string file_path) override;

protected:
    BitSet<(uint64_t)VERTEX_SIZE * K_SIZE * 32> encode_;

};


#endif //VEND_BFILTER_BIT_ENCODE_H
