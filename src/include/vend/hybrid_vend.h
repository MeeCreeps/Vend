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
    HybridVend(const std::vector<std::set<uint32_t >> &adj_list, const std::string &encode_path,DbEngine *db) : RangeVend(
            adj_list, encode_path,db) {
        Init(db);
    }
    void Init(DbEngine *db){
        encodes_ = new HybridEncode();
        encodes_->SetDb(db);
    }



};


#endif //VEND_HYBRID_VEND_H
