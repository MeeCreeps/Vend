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
#include "assert.h"
#include "algorithm"

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

// how many bits one vertex takes
uint32_t VERTEX_BIT_SIZE = static_cast<uint32_t>(ceil(log2(VERTEX_SIZE)));
// how many bits range size takes
uint32_t LOG_K = static_cast<uint32_t>(ceil(log(K_SIZE)));
// max integer number that fully encoding contains
uint32_t MAX_INTEGER_SIZE = static_cast<uint32_t >(PER_ENCODE_BIT_SIZE - 3 - LOG_K / VERTEX_BIT_SIZE);

class HybridEncode : public Encode {
public:
    enum class BlockType {
        LeftMost, RightMost, Middle
    };
    enum class EncodeType {
        UnFull = 0, Full = 1, NonDecodable = 2
    };

    HybridEncode() : log_k_(LOG_K), v_bits_size_(VERTEX_BIT_SIZE), max_integer_size_(MAX_INTEGER_SIZE), Encode() {}

    // construction for inserting and deleting
    HybridEncode(Db *adj_db) : log_k_(LOG_K), v_bits_size_(VERTEX_BIT_SIZE), max_integer_size_(MAX_INTEGER_SIZE),
                               adj_db_(adj_db), Encode() {}

    /** F()
     *  @return true, if both hybrid functions is true
     * */
    bool NEpairTest(const uint32_t &vertex1, const uint32_t &vertex2) override {
        return NonNeighborTest(vertex1, vertex2) && NonNeighborTest(vertex2, vertex1);
    };

    // f()
    bool NonNeighborTest(const uint32_t &vertex1, const uint32_t &vertex2) {
        auto &encode = encode_bitset_[vertex1];
        if (IsDecodable(vertex1)) {
            for (uint32_t index = 1; index <= PER_ENCODE_BIT_SIZE - v_bits_size_ + 1; index += v_bits_size_) {
                if (encode_bitset_[vertex1].BlockGet(index, v_bits_size_) == vertex2)
                    return false;
            }
            return true;
        } else {
            std::set<uint32_t> encode_integers = GetBlockInteger(vertex1);
            BlockType type = GetBlockModel(vertex1);
            int min = type == BlockType::LeftMost ? 0 : *encode_integers.begin();
            int max = type == BlockType::RightMost ? vertex_id_upper_ : *--encode_integers.end();
            // return true  if hash val = 0
            if (vertex2 < min || vertex2 > max)
                return !encode_bitset_[vertex1].IsOne(Hash(vertex2, GetHashBegin(vertex1)));
            else {
                for (auto &v:encode_integers) {
                    if (v == vertex2)
                        return false;
                }
                return true;
            }
        }
    };

    /**
     *  dynamic encode
     *  call function TestNonPair to choose best range size;
     *
     * */
    void EncodeVertex(const uint32_t &vertex_id, const std::set<uint32_t> &neighbors) override {
        auto &encode = encode_bitset_[vertex_id];
        if (neighbors.size() <= K_SIZE) {
            encode.SetOne(0);
            uint32_t index = 1;
            for (auto iter = neighbors.begin(); iter != neighbors.end(); ++iter, index += v_bits_size_)
                encode.BlockSet(index, v_bits_size_, *iter);
        } else
            encode = DynamicChoose(vertex_id, neighbors);
    };


    /**
     *  insert function
     *
     * */
    void InsertPair(uint32_t vertex1, uint32_t vertex2) {
        if (!NEpairTest(vertex1, vertex2))
            return;
        EncodeType encode_type1 = GetEncodeType(vertex1);
        EncodeType encode_type2 = GetEncodeType(vertex2);
        if (encode_type1 < encode_type2) {
            std::swap(vertex1, vertex2);
            std::swap(encode_type1, encode_type2);
        }
        std::set<uint32_t> neighbor1 = GetAllNeighbors(vertex1);
        std::set<uint32_t> neighbor2 = GetAllNeighbors(vertex2);
        neighbor1.insert(vertex2);
        switch (encode_type1) {
            case EncodeType::UnFull:
                EncodeVertex(vertex1, neighbor1);
                return;
            case EncodeType::Full:
                if (encode_type2 == EncodeType::Full) {
                    RestructChoose(vertex1, neighbor1, vertex2, neighbor2);
                } else {
                    EncodeVertex(vertex1, neighbor1);
                }
                return;
            case EncodeType::NonDecodable:
                RestructChoose(vertex1, neighbor1, vertex2, neighbor2);
                return;
        }
    }

    /**
     *   choose highest score between vertex1 and vertex2
     * */
    void
    RestructChoose(const uint32_t &vertex1, const std::set<uint32_t> &neighbor1, const uint32_t &vertex2,
                   const std::set<uint32_t> &neighbor2) {
        uint32_t score1, score2;
        auto bitset1 = DynamicChoose(vertex1, neighbor1, &score1);
        auto bitset2 = DynamicChoose(vertex2, neighbor2, &score2);
        if (score1 > score2)
            encode_bitset_[vertex1] = bitset1;
        else
            encode_bitset_[vertex2] = bitset2;
    }


    void DeletePair(uint32_t vertex1, uint32_t vertex2) {
        if (!NonNeighborTest(vertex1, vertex2))
            RemoveNeighbor(vertex1, vertex2);
        if (!NonNeighborTest(vertex2, vertex1))
            RemoveNeighbor(vertex2, vertex1);
    }

    // return false if vertex2 is not a neighbor
    bool RemoveNeighbor(const uint32_t &vertex1, const uint32_t &vertex2) {
        std::set<uint32_t> neighbors = GetAllNeighbors(vertex1);
        auto iter = std::find(neighbors.begin(), neighbors.end(), vertex2);
        if (iter == neighbors.end())
            return false;
        neighbors.erase(iter);
        EncodeVertex(vertex1, neighbors);
        return true;
    }

    /**
     *  dynamic choose for best range size
     *
     * */
    // TODO
    BitSet<PER_ENCODE_BIT_SIZE>
    DynamicChoose(const uint32_t &vertex_id, const std::set<uint32_t> &neighbors, uint32_t *count = nullptr);

    /**
    *  count NePair for the encode
    *  @param  vertex_id
    *          encode : dynamic encoding for vertex_id
    *  @return  vertex range from 1 to vertex size, return how many certain non-pair within this encode
    *
    * */
    //TODO
    uint32_t NePairCount(const uint32_t &vertex_id, const BitSet<PER_ENCODE_BIT_SIZE> &encode);

    /**
     *  hash function for encode
     * @return : bit position to set
     * */
    inline uint32_t Hash(const uint32_t &vertex_id, const uint32_t &hash_begin) {
        return vertex_id % (PER_ENCODE_BIT_SIZE - hash_begin) + hash_begin;
    };


    /**
     *  functions to get basic information of encode
     *  true if encode all neighbors by integers
     * */
    inline bool IsDecodable(const uint32_t &vertex) {
        return encode_bitset_[vertex].IsOne(0);
    }


    BlockType GetBlockModel(const uint32_t &vertex) {
        if (encode_bitset_[vertex].IsOne(1))
            return BlockType::RightMost;
        if (!encode_bitset_[vertex].IsOne(2))
            return BlockType::LeftMost;
        return BlockType::Middle;
    };

    void SetBlockModel(const uint32_t &vertex, BlockType type) {
        switch (type) {
            case BlockType::LeftMost:
                encode_bitset_[vertex].BlockSet(1, 2, 0);
                return;
            case BlockType::Middle:
                encode_bitset_[vertex].BlockSet(1, 2, 1);
            case BlockType::RightMost:
                encode_bitset_[vertex].BlockSet(1, 2, 3);
                return;
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

    std::set<uint32_t> GetBlockInteger(const uint32_t &vertex) {
        std::set<uint32_t> block_integers;
        if (encode_bitset_[vertex].IsOne(0)) {
            for (uint32_t index = 1;
                 index <= PER_ENCODE_BIT_SIZE - v_bits_size_ + 1; index += v_bits_size_) {
                uint32_t neighbor = encode_bitset_[vertex].BlockGet(index, v_bits_size_);
                if (!neighbor)
                    return block_integers;
                block_integers.insert(neighbor);
            }

        } else {
            for (uint32_t index = 3 + log_k_;
                 index < 3 + log_k_ + GetBlockSize(vertex) * v_bits_size_; index += v_bits_size_) {
                block_integers.insert(encode_bitset_[vertex].BlockGet(index, v_bits_size_));
            }
            return block_integers;
        }

    }


    EncodeType GetEncodeType(const uint32_t &vertex) {
        if (!IsDecodable(vertex))
            return EncodeType::NonDecodable;
        if (IsFull(vertex))
            return EncodeType::Full;
        return EncodeType::UnFull;
    }

    std::set<uint32_t> GetAllNeighbors(const uint32_t &vertex) {
        if (encode_bitset_[vertex].IsOne(0))
            return GetBlockInteger(vertex);
        return DbQuery(vertex);
    }

    inline uint32_t GetHashBegin(const uint32_t &vertex) {
        return GetBlockSize(vertex) * v_bits_size_ + 3 + log_k_;
    }

    uint32_t HashCount(const uint32_t &vertex, const uint32_t &begin, const uint32_t &hash_begin);


    /**
     *  functions to handle data from db
     *
     * */
    void EncodePersistent(Db *encode_db) override;

    // load all encode from db
    void LoadFromDb(Db *encode_db) override;

    //query neighbors from adj db
    std::set<uint32_t> DbQuery(const uint32_t &vertex);

private:
    // vertex id starts with 1
    BitSet<PER_ENCODE_BIT_SIZE> encode_bitset_[VERTEX_SIZE + 1];
    uint32_t v_bits_size_;  // how many bits one vertex takes
    uint32_t log_k_;    // how many bits range size takes
    uint32_t max_integer_size_; // max integer number that fully encoding contains
    Db *adj_db_;


};


#endif //VEND_DYNAMIC_ENCODE_H
