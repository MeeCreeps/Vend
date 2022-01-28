//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_UTILS_H
#define VEND_UTILS_H

#include <vector>
#include <cstdint>

class Utils{

public:

    static bool binary_erase(std::vector<uint32_t> &neighbors, const uint32_t &vertex) {
        int begin = 0, end = neighbors.size() - 1;
        while (begin <= end) {
            int mid = (begin + end) / 2;
            if (neighbors[mid] == vertex) {
                neighbors.erase(neighbors.begin() + mid);
                return true;
            } else if (neighbors[mid] < vertex)
                begin = mid + 1;
            else
                end = mid - 1;
        }
        return false;

    }



};



#endif //VEND_UTILS_H
