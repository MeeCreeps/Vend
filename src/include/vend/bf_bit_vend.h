//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_BF_BIT_VEND_H
#define VEND_BF_BIT_VEND_H


#include "vend.h"
#include "encode/bfilter/bfilter_bit_encode.h"
class BFilterBitVend : public Vend {

public:
    BFilterBitVend(std::shared_ptr<std::vector<std::vector<uint32_t >>> &adj_list, const std::string &encode_path,std::shared_ptr<DbEngine> &db)
            : Vend(adj_list, encode_path,db){
        encodes_ = std::make_shared<BFilterBitEncode>();
        encodes_->SetDb(db);
    }

    void BuildEncoding() override {
        assert(adjacency_list_->size() == VERTEX_SIZE + 1);
        assert(encodes_ != nullptr);
        for (uint32_t vertex1 = 1; vertex1 < adjacency_list_->size(); ++vertex1) {
            for (auto &vertex2:adjacency_list_->at(vertex1)) {
                if (vertex1 < vertex2)
                    encodes_->EdgeSet(vertex1, vertex2);
            }
        }
    }


    void Delete(uint32_t vertex1, uint32_t vertex2) override {
        LoadDbData();
        encodes_->Clear();
        BuildEncoding();
    };


};


#endif //VEND_BF_BIT_VEND_H
