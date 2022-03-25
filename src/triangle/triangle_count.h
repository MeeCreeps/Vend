//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_TRIANGLE_H
#define VEND_TRIANGLE_H

#include "vend/hybrid_vend.h"
#include "util/timer.h"
#include "dbengine/rocksdb.h"
#define VEND_LEVEL 0

class TriangleCount {
public:

    TriangleCount(std::string adj_db_path, std::string encode_path, std::string output_path, std::string edge_path,
                  uint32_t vertex_id_upper, uint32_t vertex_id_min) : adj_db_path_(
            adj_db_path), encode_path_(encode_path), output_path_(output_path), count_(0), vertex_id_upper_(
            vertex_id_upper), vertex_id_min_(vertex_id_min) {
        this->Init();
    }

    ~TriangleCount() = default;

    void Init();

    bool BinarySearch(const std::vector<uint32_t> &neighbors, int val);


    void CountByAdj();

    void CountByIntersection();

    uint32_t
    InterSection(const std::vector<uint32_t> &v_neighbors, const std::vector<uint32_t> &u_neighbors, uint32_t min);

    void OutputMessage();


private:

    struct VendMessage {
        uint64_t adj_filtered = 0;
        uint64_t edge_filtered=0;
        uint64_t total_edges=0;
    };

    Timer timer_;
    Timer search_timer_;
    std::string encode_path_;
    std::string adj_db_path_;
    std::string output_path_;
    std::shared_ptr<DbEngine> adj_db_;
    std::shared_ptr<Vend> vend_;
    std::vector<int> degrees_;
    uint32_t vertex_id_upper_;
    uint32_t vertex_id_min_;
    uint64_t count_;
    VendMessage vend_message_;
};


#endif //VEND_TRIANGLE_H
