//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_HYBRID_VEND_H
#define VEND_HYBRID_VEND_H

#include "vend/range_vend.h"
#include "encode/single/hybrid_encode.h"
#include <map>
#include <omp.h>

class HybridVend : public RangeVend {

public:
    HybridVend(std::shared_ptr<std::vector<std::vector<uint32_t >>> &adj_list, const std::string &encode_path,
               std::shared_ptr<DbEngine> &db)
            : RangeVend(adj_list, encode_path, db) {

    }

    HybridVend(const std::string &encode_path, std::shared_ptr<DbEngine> &db) : RangeVend(encode_path, db) {
        adjacency_list_= nullptr;
    }

    void Init(std::shared_ptr<DbEngine> &db) {
        encodes_ = nullptr;
        encodes_ = std::make_shared<HybridEncode>();
        encodes_->SetDb(db);
    }


};


#endif //VEND_HYBRID_VEND_H
