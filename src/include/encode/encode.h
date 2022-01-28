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
#include "assert.h"
#include <set>
#include "common/config.h"
#include "dbengine/dbengine.h"

class Encode {

public:
    Encode() : vertex_id_upper_(VERTEX_SIZE + 1), vertex_size_(VERTEX_SIZE) {}

    /**
     *  test whether vertex2 is a neighbor to vertex1
     *  @return  true if vertex2 is a neighbor to vertex1
     * */
    virtual PairType NEpairTest(uint32_t vertex1, uint32_t vertex2) = 0;

    // encode edge<vertex1,vertex2>
    virtual void EdgeSet(uint32_t vertex1, uint32_t vertex2) {};

    /**
     *  encode one vertex by it's neighbors
     *
     * */
    virtual void EncodeVertex(const uint32_t &vertex_id, std::vector<uint32_t> &neighbors) {};

    virtual void InsertPair(uint32_t vertex1, uint32_t vertex2) = 0;

    virtual void DeletePair(uint32_t vertex1, uint32_t vertex2) {};

    virtual void Clear() {};

    /**
     *  load encode from database
     *  @param encode_db_: database to store the encodes
     * */
    virtual void LoadFromDb(DbEngine *encode_db) {};

    virtual void LoadFromDb(std::string file_path) {};

    /**
     *  write encode into database
     * */
    virtual void EncodePersistent(DbEngine *encode_db) {};


    virtual void EncodePersistent(std::string file_path) {};

    virtual uint32_t GetIntSize() {};

    void SetDb(DbEngine *db){
        db_=db;

    }
    //query neighbors from adj db
    std::vector<uint32_t> DbQuery(const uint32_t &vertex) {
        assert(db_ != nullptr);
        std::vector<uint32_t> neighbors;
        db_->Get(vertex, &neighbors);
        return neighbors;
    };


protected:
    // vertex id range from [1,max_vid_)
    uint32_t vertex_id_upper_;
    // equals vertex_id_upper_ -1
    uint32_t vertex_size_;
    DbEngine *db_;
};


#endif //VEND_ENCODE_H
