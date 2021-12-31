//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_VEND_H
#define VEND_VEND_H

#include <strmif.h>
#include "encode/encode.h"
#include "encode/encode_factory.h"
#include "assert.h"

class Vend {
public:
    Vend(EncodeType encode_type, const std::vector<std::vector<uint32_t >> &adj_list, Db *encode_db) : encode_type_(
            encode_type), encode_db_(encode_db), adjacency_list_(adj_list) {
        encode_ = EncodeFactory::GetEncode(encode_type);
    }

    ~Vend() {
        delete encode_;
    }

    /**
     *  execute encoding for the graph
     * */
    void BuildEncoding();


    void LoadFromDb() {
        assert(encode_ == nullptr);
        encode_->EncodePersistent(encode_db_);
    }

    /**
    *  write encode into database
    * */
    void EncodePersistent() {
        assert(encode_ == nullptr);
        encode_->EncodePersistent(encode_db_);
    };

    /**
     *  @return 1 : if vertex1-vertex2 is a  edge exists in our encoding
     *          0 : if vertex1-vertex2 is a  edge not exists in our coding
     *          2:  uncertain edge (select by db next )
     * */
    int Determine(const uint32_t &vertex1, const uint32_t &vertex2) {
        return encode_->Get(vertex1, vertex2);
    };

private:
    Db *encode_db_;
    std::vector<std::vector<uint32_t >> adjacency_list_;
    Encode *encode_ = nullptr;
    EncodeType encode_type_;
};


#endif //VEND_VEND_H
