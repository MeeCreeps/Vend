//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_ENCODEFACTORY_H
#define VEND_ENCODEFACTORY_H

#include "encode/bloom_filter_encode.h"
#include "encode/hybrid_encode.h"
enum class EncodeType {
    BloomFilter, DynamicRangeHash
};
class EncodeFactory{
public:
    /**
     *  choose encode method by vend type
     *
     *
     * */
    static inline Encode* GetEncode(EncodeType);



};
#endif //VEND_ENCODEFACTORY_H
