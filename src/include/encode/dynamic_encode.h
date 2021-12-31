//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_DYNAMIC_ENCODE_H
#define VEND_DYNAMIC_ENCODE_H

#include "util/bitset.h"
#include "common/config.h"
#include "encode/encode.h"
#include "math.h"

/**
 *  encode format (bit)
 *  ------------------------------------------------------------------------------------------------------------------
 *  | FULL FLAG (1) | RANGE MODEL (2) | RANGE SIZE (log(k))|  RANGE ENCODE PART (RANGE SIZE * VERTEX BIT)| HASH ENCODE PART
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *
 *  FULL FLAG (1 bit) :  1 represent fully encode, 0 for partially encode
 *  RANGE MODEL(2 bits) :  00  ——  range part contains minimal id (0)
 *                   01  ——  range part contains neither minial id or maximal id
 *                   10  ——  range part contains  maximal id (VERTEX_SIZE + 1)
 *  RANGE SIZE (log(k) bits) : represent how many integers for range encode
 *  VERTEX BIT  :  how many bits one vertex id takes      ( log(|V|))
 * */


class DynamicEncode : public Encode {
public:

    bool Get(const uint32_t &vertex1, const uint32_t &vertex2) override;

    /**
     *  dynamic encode
     *  call function TestNonPair to choose best range size;
     *
     * */
    void EncodeVertex(const uint32_t &vertex_id, const std::vector<uint32_t> &neighbors) override;

    /**
     *  dynamic choose for best range size
     *  @param  vertex_id
     *          encode : dynamic encoding for vertex_id
     *  @return  vertex range from 1 to vertex size, return how many certain non-pair within this encode
     *
     * */
    uint32_t TestNonPair(const uint32_t &vertex_id, const BitSet<PER_ENCODE_BIT_SIZE> &encode);


    inline uint32_t Hash(const uint32_t &vertex_id, const uint32_t hash_range) {
        return vertex_id % hash_range;
    };

    void EncodePersistent(Db *encode_db_) override;

    void LoadFromDb(Db *encode_db_) override;

private:
    BitSet<PER_ENCODE_BIT_SIZE> encode_bitset_[VERTEX_SIZE];
    uint32_t v_bits_size_ = VERTEX_BIT_SIZE;  // how many bits one vertex takes
    uint32_t log_k_size_ = LOG_K_SIZE;    // how many bits range size takes
    uint32_t max_integer_size = MAX_INTEGER_SIZE; // max integer number that fully encoding contains

};


#endif //VEND_DYNAMIC_ENCODE_H
