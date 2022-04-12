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

#include "encode/single/hybrid_encode.h"
#include "util/timer.h"
#include "dbengine/rocksdb.h"
#include "dbengine/neo4j.h"

#define VEND_LEVEL 0

class TriangleCount {
public:
    struct time_message{
        uint32_t id;
        int degree;
        double time;
    };
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

    void OutTimer(std::vector<time_message>& node_time);

    void ThreadDecode(int tid,int thread_num,int degree,std::vector<uint32_t>& vertex_id, HybridEncode::DecodeInfo *decodeInfo);

    int GetThreadNum(int degree);
private:

    struct VendMessage {
        uint64_t adj_filtered = 0;
        uint64_t total_adj=0;
        uint64_t edge_filtered=0;
        uint64_t total_edges=0;

        VendMessage operator+=(const VendMessage &rhs){
            total_edges+=rhs.total_edges;
            edge_filtered+=rhs.edge_filtered;
            total_edges+=rhs.total_edges;
        }

    };

    Timer timer_;
    std::string encode_path_;
    std::string adj_db_path_;
    std::string output_path_;
    std::shared_ptr<DbEngine> adj_db_;
    std::shared_ptr<HybridEncode> encode_;
    std::vector<int> degrees_;
    uint32_t vertex_id_upper_;
    uint32_t vertex_id_min_;
    int max_degree_=0;
    uint64_t count_;
    VendMessage encode_message_;
};


#endif //VEND_TRIANGLE_H
