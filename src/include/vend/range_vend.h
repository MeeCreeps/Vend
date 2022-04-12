//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_RANGE_VEND_H
#define VEND_RANGE_VEND_H

#include "vend.h"
#include "encode/single/range_encode.h"
class RangeVend:public Vend{


public:
    RangeVend(const std::vector<std::vector<uint32_t >> &adj_list, const std::string &encode_path,DbEngine *db) : Vend(
            adj_list,encode_path,db) {
        Init(db);
    }

    virtual void Init(DbEngine *db){
        encodes_ = new RangeEncode();
        encodes_->SetDb(db);
    }
    void BuildEncoding() override;


protected:

    struct K_Node {
        uint32_t id;
        uint32_t neighbors_num;
        bool operator==(const K_Node &rhs)const{
            return id==rhs.id&&neighbors_num==rhs.neighbors_num;
        }
        bool operator<(const K_Node &rhs) const {
            if( this->neighbors_num==rhs.neighbors_num)
                return id<rhs.id;
            return this->neighbors_num < rhs.neighbors_num;
        }

    };




};


#endif //VEND_RANGE_VEND_H
