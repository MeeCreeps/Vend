//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_BLOOM_FILTER_ENCODE_H
#define VEND_BLOOM_FILTER_ENCODE_H

#include "util/bitset.h"
#include "common/config.h"
#include "encode/encode.h"

class BFilterEncode : public Encode {
public:

    /**
     *  hash function for bloom filter
     *  @param key : input key
     *          hash param
     *  @return :  return (key*hash_param1+hash_param2)%|bitset|
     * */
    inline uint32_t Hash(uint32_t key, uint32_t hash_param1, uint32_t hash_param2);

    bool Get(const uint32_t &vertex1, const uint32_t &vertex2) override;

    void Set(const uint32_t &vertex1, const uint32_t &vertex2) override;

    void LoadFromDb(Db *encode_db_) override;

    void EncodePersistent(Db *encode_db_) override;

private:
    BitSet<B_FILTER_ENCODE_BITS> encode_bitset_;
    uint32_t h1_param1 = BLOOM_FILTER_HASH1_PARAM1;
    uint32_t h1_param2 = BLOOM_FILTER_HASH1_PARAM2;
    uint32_t h2_param1 = BLOOM_FILTER_HASH2_PARAM1;
    uint32_t h2_param2 = BLOOM_FILTER_HASH2_PARAM2;
    uint32_t h3_param1 = BLOOM_FILTER_HASH3_PARAM1;
    uint32_t h3_param2 = BLOOM_FILTER_HASH3_PARAM2;

};

#endif //VEND_BLOOM_FILTER_ENCODE_H
