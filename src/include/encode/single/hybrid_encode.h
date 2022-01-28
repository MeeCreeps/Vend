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


#include "math.h"
#include "assert.h"
#include "algorithm"

#include "encode/hash_count.h"
#include "util/bitset.h"
#include "common/config.h"
#include "encode/single/encode_bitset.h"
#include "dbengine/dbengine.h"
#include "common/config.h"
#include "util/utils.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <set>
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
 *                   11  ——  range part contains  maximal id (VERTEX_SIZE + 1)
 *  RANGE SIZE (log(k) bits) : represent how many integers for range encode
 *  VERTEX BIT  :  how many bits one vertex id takes      ( log(|V|))
 * */




class HybridEncode : public EncodeBitSet {
public:


    HybridEncode() : log_k_(LOG_K), v_bits_size_(VERTEX_BIT_SIZE), max_integer_size_(MAX_INTEGER_SIZE),
                     EncodeBitSet() {}



    /** F()
     *  @return true, if both hybrid functions is true
     * */
    PairType NEpairTest(uint32_t vertex1, uint32_t vertex2) override {

        PairType type1=NonNeighborTest(vertex1, vertex2);
        PairType type2=NonNeighborTest(vertex2, vertex1);

        if(type1<type2)
            return type1;
        else
            return type2;
    };

    // f()
    PairType NonNeighborTest(const uint32_t &vertex1, const uint32_t &vertex2) override;

    /**
     *  dynamic encode
     *  call function TestNonPair to choose best range size;
     *
     * */
    void EncodeVertex(const uint32_t &vertex_id, std::vector<uint32_t> &neighbors) override;


    /**
     *  insert function
     *
     * */
    void InsertPair(uint32_t vertex1, uint32_t vertex2) override;

    /**
     *   choose highest score between vertex1 and vertex2
     * */
    void
    RestructChoose(const uint32_t &vertex1, std::vector<uint32_t> &neighbor1, const uint32_t &vertex2,
                   std::vector<uint32_t> &neighbor2);


    void DeletePair(uint32_t vertex1, uint32_t vertex2) override;

    // return false if vertex2 is not a neighbor
    bool RemoveNeighbor(const uint32_t &vertex1, const uint32_t &vertex2);

    /**
     *  dynamic choose for best range size
     *
     * */

    BitSet<PER_ENCODE_BIT_SIZE>
    DynamicChoose(const uint32_t &vertex, std::vector<uint32_t> &neighbors, uint32_t *count = nullptr);

    // compare score and choose the highest, and build the encode
    void
    ChooseHighest(const std::vector<uint32_t> &neighbors, uint32_t *max_score,
                  BitSet<PER_ENCODE_BIT_SIZE> *best_bitset,
                  HashCount *hash_count,
                  const uint32_t &block_size, const uint32_t &first_idx,
                  const BlockType &type);

    void SetNonDecodable(const std::vector<uint32_t> &neighbor_encode, const std::vector<uint32_t> &neighbor_left,
                         const BlockType &type,
                         BitSet<PER_ENCODE_BIT_SIZE> *bitset);

    /**
    *  count NePair for the encode
    *  @param  vertex_id
    *          encode : dynamic encoding for vertex_id
    *  @return  vertex range from 1 to vertex size, return how many certain non-pair within this encode
    *
    * */

    uint32_t NePairCount(const uint32_t &vertex_id, const BitSet<PER_ENCODE_BIT_SIZE> &encode);


    /**
     *  hash function for encode
     * @return : bit position to set
     * */
    inline uint32_t Hash(const uint32_t &vertex_id, const uint32_t &hash_begin) {
        return hash_begin == PER_ENCODE_BIT_SIZE ? 0 : vertex_id % (PER_ENCODE_BIT_SIZE - hash_begin) + hash_begin;
    };


    /**
     *  functions to get basic information of encode
     *  true if encode all neighbors by integers
     * */


    inline bool IsDecodable(const uint32_t &vertex) {
        return encode_bitset_[vertex].IsOne(0);
    }

    /**
     *  return encode integers
     * */
    std::vector<uint32_t> GetBlockInteger(const uint32_t &vertex);

    BlockType GetBlockModel(const uint32_t &vertex) {
        if (encode_bitset_[vertex].IsOne(1))
            return BlockType::RightMost;
        if (!encode_bitset_[vertex].IsOne(2))
            return BlockType::LeftMost;
        return BlockType::Middle;
    };

    void SetBlockModel(BitSet<PER_ENCODE_BIT_SIZE> *bitset, BlockType type) {
        switch (type) {
            case BlockType::LeftMost:
                bitset->BlockSet(1, 2, 0);
                break;
            case BlockType::FullHash:
            case BlockType::Middle:
                bitset->BlockSet(1, 2, 1);
                break;
            case BlockType::RightMost:
                bitset->BlockSet(1, 2, 3);
                break;
        }
    }


    inline bool IsFull(const uint32_t &vertex) {
        assert(encode_bitset_[vertex].IsOne(0));
        return encode_bitset_[vertex].BlockGet(PER_ENCODE_BIT_SIZE - v_bits_size_, v_bits_size_) != 0;
    }

    inline bool IsFull(const uint32_t &vertex, uint32_t *index_not_full) {
        assert(encode_bitset_[vertex].IsOne(0));
        for (uint32_t index = 1;
             index <= PER_ENCODE_BIT_SIZE - v_bits_size_ + 1; index += v_bits_size_) {
            if (encode_bitset_[vertex].BlockGet(index, v_bits_size_) == 0)
                return false;
        }
        return true;
    }

    inline uint32_t GetBlockSize(const uint32_t &vertex) {
        return encode_bitset_[vertex].BlockGet(3, log_k_);
    }


    EncodeType GetEncodeType(const uint32_t &vertex) {
        if (!IsDecodable(vertex))
            return EncodeType::NonDecodable;
        if (IsFull(vertex))
            return EncodeType::Full;
        return EncodeType::UnFull;
    }

    std::vector<uint32_t> GetAllNeighbors(const uint32_t &vertex) {
        if (encode_bitset_[vertex].IsOne(0))
            return GetBlockInteger(vertex);
        return DbQuery(vertex);
    }

    inline uint32_t GetHashBegin(const uint32_t &vertex) {
        return GetBlockSize(vertex) * v_bits_size_ + 3 + log_k_;
    }

    uint32_t GetIntSize() { return max_integer_size_; }

protected:

    uint32_t v_bits_size_;  // how many bits one vertex takes
    uint32_t log_k_;    // how many bits range size takes
    uint32_t max_integer_size_; // max integer number that fully encoding contains


};


#endif //VEND_DYNAMIC_ENCODE_H
