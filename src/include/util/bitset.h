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
// round up to x/32
#define ROUND_UP(X)= x%32==0 ?x/32:x/32+1

template<size_t N>
class BitSet {
public:

    /**
 *  set bit on  position pos to 1
 *  @return   false if pos out of range
 * */
    bool SetOne(size_t pos);

    // reset pos as 0
    bool SetZero(size_t pos);

    /**
     *  test whether bit on position pos is equal 1
     *  @return  true: equal 1   false
     * */
    bool IsOne(size_t pos);

    // reset all as 0
    bool Clear();

    /**
     *  set bits from begin with block_size length  equals value
     *  @param  begin: blockset position begins    [0,32*N-1]
     *          block_size: gap size
     *          value:
     *  @example   blockSet(5,5,6);
     *  bitset before: 00000 00000 00000 ...
     *         after:  00000 01100 00000 ...
     * */
    void BlockSet(size_t begin, size_t block_size, uint32_t value);

    /**
     *  apply blockset foreach one in the array
     *  @param begin: beginning position
     *         block_size: how many bits that one takes (equals ceil(log|V|) )
     *         values:  value array
     * */
    void BlockSet(size_t begin, size_t block_size, std::vector<uint32_t> values);

    void BlockSet(size_t begin, size_t block_size, std::set<uint32_t> values);

    /**
     *  convert bits to integer (uint32)
     *  @param  begin: beginning position
     *          block_size: how many bit that one takes
     *  @example    ConverToInt(2,10);
     *         bitset: 00000 01010 ...
     *         return: 10
     * */
    uint32_t BlockGet(size_t begin, size_t block_size);

    /**
     *
     *
     * */
    void BlockGet(size_t begin, size_t block_size, std::vector<uint32_t> *intVec);

    void BlockGet(size_t begin, size_t block_size, std::set<uint32_t> *intVec);

    /**
     *  return how many bits are set range from begin to begin+block_size
     *
     * */
    uint32_t BlockCount(size_t begin, size_t block_size);

private:

    uint32_t bits_[ ROUND_UP(N)
    ];
    size_t bit_nums_ = N;
};


#endif //VEND_BITSET_H
