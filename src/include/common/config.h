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
#include "stdint.h"
#include <string>

static constexpr int K_SIZE = 5;
// 1696415 1791489 3072441
static constexpr uint32_t VERTEX_SIZE = 1791489;        // how many vertices
// 13 28 76
static constexpr uint32_t DEGREE = 28;

// how many bits bloom filter for the graph takes
static constexpr uint32_t B_FILTER_ENCODE_BITS = VERTEX_SIZE * K_SIZE;
static constexpr int PER_ENCODE_BIT_SIZE = K_SIZE * 32;
// how many bits one vertex takes
static uint32_t VERTEX_BIT_SIZE = static_cast<uint32_t>(ceil(log2((double) VERTEX_SIZE)));


static uint32_t MAX_K_CORE_SIZE = static_cast<uint32_t >((PER_ENCODE_BIT_SIZE - 1) / VERTEX_BIT_SIZE);
// how many bits range size takes
static uint32_t LOG_K = static_cast<uint32_t>(ceil(log2((double) MAX_K_CORE_SIZE)));

// max integer number that fully encoding contains
static uint32_t MAX_INTEGER_SIZE = static_cast<uint32_t >((PER_ENCODE_BIT_SIZE - 3 - LOG_K) / VERTEX_BIT_SIZE);

static constexpr int THREAD_NUMS = 1;
enum  PairType{
    Neighbor=1,NonNeighbor=2,Uncertain=4

};
#endif //VEND_CONFIG_H
