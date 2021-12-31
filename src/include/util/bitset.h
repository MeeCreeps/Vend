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

// round up to x/32
#define ROUND_UP(X)= x%32==0 ?x/32:x/32+1

template<size_t N>
class BitSet {
public:

    /**
     *  set bits from begin with bit_size length  equals value
     *  @param  begin: gapset position begins    [0,32*N-1]
     *          bit_size: gap size
     *          value:
     *  @example   GetSet(5,5,6);
     *  bitset before: 00000 00000 00000 ...
     *         after:  00000 01100 00000 ...
     * */
    void GapSet(size_t begin, size_t bit_size, uint32_t value)

    /**
     *  apply gapset foreach one in the array
     *  @param begin: beginning position
     *         each_size: how many bits that one takes (equals log|V|)
     *         values:  value array
     * */
    void BulkGapSet(size_t begin, size_t each_size, std::vector<uint32_t> values)

    /**
     *  convert bits to integer (uint32)
     *  @param  begin: beginning position
     *          bit_size: how many bit that one takes
     *  @example    ConverToInt(2,10);
     *         bitset: 00000 01010 ...
     *         return: 10
     * */
    uint32_t ConvertToInt(size_t begin, size_t bit_size)

    /**
     *  convert bitset to int array
     *  @return vector size : N
     * */
    std::vector<uint32_t> ConvertToIntArray()

    /**
     *  test whether bit on position pos is equal 1
     *  @return  true: equal 1   false
     * */
    bool Test(size_t pos)


    /**
     *  return how many bits are set range from begin to begin+gap
     *
     * */
    uint32_t GapTest(size_t begin,size_t gap)


    /**
     *  set bit on  position pos to 1
     *  @return   false if pos out of range
     * */
    bool Set(size_t pos)


private:

    uint32_t bits_[ROUND_UP(N)];
    size_t bit_nums_ = N;
};


#endif //VEND_BITSET_H
