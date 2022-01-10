//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_CONFIG_H
#define VEND_CONFIG_H

#include "math.h"

static constexpr int K_SIZE = 5;
static constexpr uint32_t VERTEX_SIZE = 1000000;        // how many vertices
// how many bits bloom filter for the graph takes
static constexpr uint32_t B_FILTER_ENCODE_BITS = VERTEX_SIZE * K_SIZE;
static constexpr int PER_ENCODE_BIT_SIZE = K_SIZE * 32;
static constexpr uint32_t BLOOM_FILTER_HASH1_PARAM1 = 9191891;
static constexpr uint32_t BLOOM_FILTER_HASH1_PARAM2 = 1635947;
static constexpr uint32_t BLOOM_FILTER_HASH2_PARAM1 = 3877847;
static constexpr uint32_t BLOOM_FILTER_HASH2_PARAM2 = 2908361;
static constexpr uint32_t BLOOM_FILTER_HASH3_PARAM1 = 1635148;
static constexpr uint32_t BLOOM_FILTER_HASH3_PARAM2 = 9191891;



#endif //VEND_CONFIG_H
