//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_BIT_HASH_VEND_H
#define VEND_BIT_HASH_VEND_H

#include "vend.h"
#include "encode/single/bithash_encode.h"

class BitHashVend : public Vend {
public:
    BitHashVend(const std::vector<std::set<uint32_t >> &adj_list, const std::string &encode_path,DbEngine *db)
            : Vend(adj_list, encode_path,db) {
        encodes_ = new BitHashEncode();
        encodes_->SetDb(db);
    }

    void BuildEncoding() override {
        assert(adjacency_list_.size() == VERTEX_SIZE + 1);
        assert(encodes_ != nullptr);
        uint32_t vertex1 = 1;
        for (uint32_t vertex1 = 1; vertex1 < adjacency_list_.size(); ++vertex1) {
            std::vector<uint32_t> neighbors(adjacency_list_[vertex1].begin(),adjacency_list_[vertex1].end());
            encodes_->EncodeVertex(vertex1,neighbors);
        }

    };

};


#endif //VEND_BIT_HASH_VEND_H
