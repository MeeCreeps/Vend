//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_BITSET_H
#define VEND_BITSET_H


#include <bitset>
#include <vector>
#include <set>
#include "assert.h"
#include <cstring>
// round up to x/32
#define ROUND_UP(x) x%32==0 ?x/32:x/32+1
#define DIVIDE32(x) x>>5
#define MULTIPLY32(x) x<<5
#define MOD32(x) x-((x>>5)<<5)

// array for bits moving
static constexpr uint32_t ONE_BITS_ARRAY[33] = {
        0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000f,
        0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff,
        0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff,
        0x00001fff, 0x00003fff, 0x00007fff, 0x0000ffff,
        0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff,
        0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff,
        0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff,
        0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff
};

static constexpr uint32_t ZERO_BITS_ARRAY[33] = {
        0xffffffff, 0xfffffffe, 0xfffffffc, 0xffffff8, 0xffffff0,
        0xffffffe0, 0xffffffc0, 0xffffff80, 0xffffff00,
        0xfffffe00, 0xfffffc00, 0xfffff800, 0xfffff000,
        0xffffe000, 0xffffc000, 0xffff8000, 0xffff0000,
        0xfffe0000, 0xfffc0000, 0xfff80000, 0xfff00000,
        0xffe00000, 0xffc00000, 0xff800000, 0xff000000,
        0xfe000000, 0xfc000000, 0xf8000000, 0xf0000000,
        0xe0000000, 0xc0000000, 0x80000000, 0x00000000
};
static constexpr uint32_t ONE_BIT[32] = {
        0x80000000, 0x40000000, 0x20000000, 0x10000000,
        0x08000000, 0x04000000, 0x02000000, 0x01000000,
        0x00800000, 0x00400000, 0x00200000, 0x00100000,
        0x00080000, 0x00040000, 0x00020000, 0x00010000,
        0x00008000, 0x00004000, 0x00002000, 0x00001000,
        0x00000800, 0x00000400, 0x00000200, 0x00000100,
        0x00000080, 0x00000040, 0x00000020, 0x00000010,
        0x00000008, 0x00000004, 0x00000002, 0x00000001,
};

template<size_t N>
class BitSet {
public:
    BitSet():size_(ROUND_UP(N)) {
        Clear();
    }

    /**
 *  set bit on  position pos to 1
 *  @return   false if pos out of range
 * */
    bool SetOne(size_t pos) {
        bits_[pos / 32] |= (uint32_t) 1 << (31 - pos % 32);
    };

    // reset pos as 0
    bool SetZero(size_t pos) {
        bits_[pos / 32] &= ~(uint32_t) 0 ^ (1 << (31 - pos % 32));
    };

    /**
     *  test whether bit on position pos is equal 1
     *  @return  true: equal 1   false
     * */
    bool IsOne(size_t pos) {
        return bits_[pos/32] & ONE_BIT[pos % 32];
    };

    bool Pos_One_Is_One() {
        return bits_[0] >> 31;
    }

    // reset all as 0
    bool Clear() {
        memset(bits_, 0, size_ * sizeof(uint32_t));
    };

    /**
     *  set bits from begin with block_size length  equals value
     *  @param  begin: blockset position begins    [0,32*N-1]
     *          block_size:block size    <32
     *          value:
     *  @example   blockSet(5,5,6);
     *  bitset before: 00000 00000 00000 ...
     *         after:  00000 01100 00000 ...
     * */
    void BlockSet(size_t begin, size_t block_size, uint32_t value) {
        uint32_t i = begin / 32;
        uint32_t left = 31 - (begin + block_size - 1) % 32;
        uint32_t j = (begin + block_size - 1) / 32;
        if (i != j) {
            bits_[i] &= ZERO_BITS_ARRAY[32 - begin % 32];
            bits_[i] |= value >> (32 - left);

            bits_[j] &= ONE_BITS_ARRAY[left];
            bits_[j] |= value << left;
        } else {
            bits_[i] &= ~(ONE_BITS_ARRAY[block_size] << left);
            bits_[i] |= value << left;
        }
    };

    void BlockSet(const std::vector<uint32_t> &encode) {
        assert(encode.size() <= size_);
        for (int i = 0; i < size_; ++i)
            bits_[i] = encode[i];
    };

    std::vector<uint32_t> Dump() {
        std::vector<uint32_t> integers;
        for (int i = 0; i < size_; ++i)
            integers.push_back(bits_[i]);
        return integers;
    }

    /**
     *  apply blockset foreach one in the array
     *  @param begin: beginning position
     *         block_size: how many bits that one takes (equals ceil(log|V|) )
     *         values:  value array
     * */
    void BlockSet(size_t begin, size_t block_size, const std::vector<uint32_t> &values) {
        size_t temp = begin;
        for (auto iter = values.begin(); iter != values.end(); ++iter, temp += block_size) {
            BlockSet(temp, block_size, *iter);
        }
    };

    void BlockSet(size_t begin, size_t block_size, const std::set<uint32_t> &values);

    /**
     *  convert bits to integer (uint32)
     *  @param  begin: beginning position
     *          block_size: how many bit that one takes
     *  @example    ConverToInt(2,10);
     *         bitset: 00000 01010 ...
     *         return: 10
     * */
    uint32_t BlockGet(size_t begin, size_t block_size) {
        // block_size <= 32
        size_t i = DIVIDE32(begin);
        uint32_t temp =begin + block_size - 1;
        size_t j = DIVIDE32(temp);
        if (i == j) {
            size_t offset = ((j+1)<<5)-temp-1;
            return (bits_[i] << offset) & ONE_BITS_ARRAY[block_size];
            //return (bits_[i] << (begin - (i<<5))) >> (32 - block_size);
        } else {
            size_t offset = ((j+1)<<5)-temp-1;
            return ((bits_[i] << (begin - (i<<5))) >> (32 - block_size)) | (bits_[j] >> offset);
        }

    };

    /**
     *  convert all integers  to vector
     *
     * */
    void BlockGet(size_t begin, size_t block_size, std::vector<uint32_t> *intVec) {
        while (begin + block_size - 1 < size_ * 32) {
            intVec->push_back(BlockGet(begin, block_size));
            begin += block_size;
        }
    };

    void BlockGet(size_t begin, size_t block_size, std::set<uint32_t> *intVec) {
        while (begin + block_size - 1 < size_ * 32) {
            intVec->insert(BlockGet(begin, block_size));
            begin += block_size;
        }
    };

    /**
     *  return how many bits are set range from begin to begin+block_size
     *
     * */
    uint32_t BlockCount(size_t begin, size_t block_size);

private:
    uint32_t bits_[ROUND_UP(N)];
    int size_;
};


#endif //VEND_BITSET_H
