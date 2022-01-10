//===----------------------------------------------------------------------===//
//
//                         
//                  Encode.h
//          super class for  encoding plan
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_ENCODE_H
#define VEND_ENCODE_H

#include <vector>
#include "common/config.h"
#include <set>
class Encode {

public:
    Encode() : vertex_id_upper_(VERTEX_SIZE + 1), vertex_size_(VERTEX_SIZE) {}

    /**
     *  test whether vertex2 is a neighbor to vertex1
     *  @return  true if vertex2 is a neighbor to vertex1
     * */
    virtual bool NEpairTest(const uint32_t &vertex1, const uint32_t &vertex2) = 0;

    // encode edge<vertex1,vertex2>
    virtual void EdgeSet(const uint32_t &vertex1, const uint32_t &vertex2);

    /**
     *  encode one vertex by it's neighbors
     *
     * */
    virtual void EncodeVertex(const uint32_t &vertex_id, const std::set<uint32_t> &neighbors);

    /**
     *  load encode from database
     *  @param encode_db_: database to store the encodes
     * */
    virtual void LoadFromDb(Db *encode_db_) = 0;

    /**
     *  write encode into database
     * */
    virtual void EncodePersistent(Db *encode_db_) = 0;

protected:
    // vertex id range from [1,max_vid_)
    uint32_t vertex_id_upper_;
    // equals vertex_id_upper_ -1
    uint32_t vertex_size_;
};


#endif //VEND_ENCODE_H
