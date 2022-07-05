
#include "encode/bfilter/dbfilter_encode.h"

PairType DBFilterEncode::NEpairTest(uint32_t vertex1, uint32_t vertex2) {
    if (vertex1 > vertex2)
        std::swap(vertex1, vertex2);
    for (int i = 0; i < best_hash_nums_; ++i) {
        if (!encode_.IsOne(Hash(vertex1, vertex2, hash_param1_[i], hash_param2_[i]) + region_size_))
            return PairType::NonNeighbor;
    }
    return PairType::Uncertain;
}

void DBFilterEncode::EdgeSet(uint32_t vertex1, uint32_t vertex2) {
    if (vertex1 > vertex2)
        std::swap(vertex1, vertex2);
    for (int i = 0; i < best_hash_nums_; ++i) {
        uint64_t hash_val = Hash(vertex1, vertex2, hash_param1_[i], hash_param2_[i]);
        if (encode_.IsOne(hash_val))
            encode_.SetOne(hash_val % region_size_);
        else
            encode_.SetOne(hash_val + region_size_);
    }


}

void DBFilterEncode::InsertPair(uint32_t vertex1, uint32_t vertex2) {
    EdgeSet(vertex1,vertex2);
}

bool DBFilterEncode::IsDeletable(uint32_t vertex1, uint32_t vertex2) {
    if (vertex1 > vertex2)
        std::swap(vertex1, vertex2);
    for (int i = 0; i < best_hash_nums_; ++i) {
        uint64_t hash_val = Hash(vertex1, vertex2, hash_param1_[i], hash_param2_[i]);
        if (encode_.IsOne(hash_val%region_size_))
            return false;
    }
    return true;
}

void DBFilterEncode::DeletePair(uint32_t vertex1, uint32_t vertex2) {
    if (vertex1 > vertex2)
        std::swap(vertex1, vertex2);
    for (int i = 0; i < best_hash_nums_; ++i) {
        uint64_t hash_val = Hash(vertex1, vertex2, hash_param1_[i], hash_param2_[i]);
        encode_.SetOne(hash_val+region_size_);
    }
}
