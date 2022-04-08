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


int main(int argc, char **argv) {
    std::string adj_db_path = argv[1];
    std::string encode_path = argv[2];
    std::string output_path = argv[3];
    std::string edge_path = argv[4];


    std::clock_t start = std::clock();
    uint32_t vertex_id_min = 1, vertex_id_upper =VERTEX_SIZE+1 ;
    std::shared_ptr<TriangleCount> tcount = std::make_shared<TriangleCount>(adj_db_path, encode_path, output_path,
                                                                            edge_path, vertex_id_upper, vertex_id_min);
#ifdef VEND_LEVEL
#if (VEND_LEVEL>=0)
    std::cout<<"count by adj \n vend level : "<<VEND_LEVEL<<" \n";
    tcount->CountByAdj();
#else
    std::cout<<"count by intersection with vend \n";
    tcount->CountByIntersection();
#endif
#else
    std::cout<<"count by intersection \n";
    tcount->CountByIntersection();
#endif
    std::clock_t end = std::clock();
    std::cout<<"total time: "<< (double)(end-start)/CLOCKS_PER_SEC<<"\n";
}
