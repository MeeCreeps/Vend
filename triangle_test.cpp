//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#include "triangle/triangle_count.h"

#define VEND_LEVEL 0
int main(int argc, char **argv) {
//    std::string adj_db_path = argv[1];
//    std::string encode_path = argv[2];
//    std::string output_path = argv[3];
//    std::string edge_path = argv[4];

    std::string adj_db_path = "/home/juvenile/Desktop/Vend/wiki/5/db";
    std::string encode_path = "/home/juvenile/Desktop/Vend/wiki/5/encode/Hybrid.txt";
    std::string output_path = "/home/juvenile/Desktop/Vend/wiki/5/result/triangle.csv";
    std::string edge_path = "/home/juvenile/Desktop/Vend/resource/wiki.txt";
    std::clock_t start =std::clock();

    uint32_t vertex_id_min = 1, vertex_id_upper =50000 ;

    std::shared_ptr<TriangleCount> tcount = std::make_shared<TriangleCount>(adj_db_path, encode_path, output_path,
                                                                            edge_path, vertex_id_upper, vertex_id_min);


#if (VEND_LEVEL < 3)
    tcount->CountByAdj();
#else
    tcount->CountByIntersection();
#endif
    std::clock_t end =std::clock();
    std::cout<<(double) (end-start)/CLOCKS_PER_SEC<<"\n";
}