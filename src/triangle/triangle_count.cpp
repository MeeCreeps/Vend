//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#include "triangle_count.h"
#include <memory>

/*
 *  @VEND_LEVEL
 *      0  :  Triangle count without vend filter
 *      1  ： use vend before binary search
 *
 * */

#define VEND_LEVEL 0

void TriangleCount::Init() {
    adj_db_ = std::make_shared<RocksDb>(adj_db_path_);
    vend_ = std::make_shared<HybridVend>(std::vector<std::set<uint32_t >>(), encode_path_, nullptr);
    vend_->LoadEncode();

    // count degree
    degrees_.resize(VERTEX_SIZE + 1);
    adj_db_->InitIter();
    uint32_t vertex_id;
    std::vector<uint32_t> neighbors;
    while (adj_db_->Next(&vertex_id, &neighbors)) {
        degrees_[vertex_id] = neighbors.size();
    }

}


void TriangleCount::CountByAdj() {
    timer_.StartTimer();

    for (uint32_t v = vertex_id_min_; v < vertex_id_upper_; ++v) {
        std::vector<uint32_t> neighbors_of_v;
        adj_db_->Get(v, &neighbors_of_v);
        int v_degree = degrees_[v];
        for (auto u:neighbors_of_v) {
            int u_degree = degrees_[u];
            if (u_degree < v_degree || (u_degree == v_degree && u <= v))
                continue;


#if (VEND_LEVEL >= 2)
            // vend test
            bool filtered=true;
            for(auto n:neighbors_of_v){
                int n_degree = degrees_[n];
                if(n_degree>u_degree || (n_degree == u_degree && n>u)){
                    if(vend_->Determine(n,u)!=PairType::NonNeighbor){
                        filtered= false;
                        break;
                    }
                }
            }
            if(filtered){
                ++vend_message_.adj_filtered;
                continue;
            }

#endif
            std::vector<uint32_t> neighbors_of_u;
            adj_db_->Get(u, &neighbors_of_u);
            for (auto n:neighbors_of_u) {
                int n_degree = degrees_[n];
                if (n_degree < u_degree || (n_degree == u_degree && n <= u))
                    continue;

                ++vend_message_.total_edges;



#if (VEND_LEVEL >= 1)
                PairType pair_type = vend_->Determine(n,v);
                if(pair_type==PairType::Neighbor) {
                    ++count_;
                    ++vend_message_.edge_filtered;
                    continue;
                }
                else if(pair_type == PairType::NonNeighbor){
                    ++vend_message_.edge_filtered;
                    continue;
                }

#endif

                if (BinarySearch(neighbors_of_v, n)) {
                    ++count_;
                }
            }
        }
    }
    timer_.StopTimer();

    OutputMessage();

}

void TriangleCount::CountByIntersection() {

    timer_.StartTimer();
    // id restrict :  v< u < intersection
    for (uint32_t v = vertex_id_min_; v < vertex_id_upper_; ++v) {
        std::vector<uint32_t> neighbors_of_v;
        adj_db_->Get(v, &neighbors_of_v);
        for (auto u:neighbors_of_v) {
            if (v >= u)
                continue;
#if (VEND_LEVEL >= 3)
            // vend test
            bool filtered=true;
            for(auto n:neighbors_of_v){
                if(n>v){

                    if(vend_->Determine(n,u)!=PairType::NonNeighbor){
                        filtered= false;
                        break;
                    }
                }
            }
            if(filtered){
                ++vend_message_.adj_filtered;
                continue;
            }

#endif
            std::vector<uint32_t> neighbors_of_u;
            adj_db_->Get(u, &neighbors_of_u);
            count_ += InterSection(neighbors_of_u, neighbors_of_v, u);
        }
    }
    timer_.StopTimer();
    OutputMessage();

}

bool TriangleCount::BinarySearch(const std::vector<uint32_t> &neighbors, int val) {
    int left = 0, right = neighbors.size() - 1;

    if (right < 0 || val < neighbors[left] || val > neighbors[right])
        return false;


    while (left <= right) {
        int mid = (left + right) / 2;
        if (neighbors[mid] == val)
            return true;
        else if (neighbors[mid] < val)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return false;
}

uint32_t
TriangleCount::InterSection(const std::vector<uint32_t> &v_neighbors, const std::vector<uint32_t> &u_neighbors,
                            uint32_t min) {
    int v_size = v_neighbors.size(), u_size = u_neighbors.size(), v_idx = 0, u_idx = 0;
    uint32_t count = 0;
    while (v_neighbors[v_idx] <= min)
        ++v_idx;
    while (u_neighbors[u_idx] <= min)
        ++u_idx;
    while (v_idx < v_size && u_idx < u_size) {
        if (v_neighbors[v_idx] == u_neighbors[u_idx]) {
            ++count;
            ++v_idx;
        } else if (v_neighbors[v_idx] < u_neighbors[u_idx])
            ++v_idx;
        else
            ++u_idx;
    }
    return count;
}

void TriangleCount::OutputMessage() {

    std::ofstream output(output_path_, std::ios::app | std::ios::out);
#ifdef VEND_LEVEL
    double total_time = timer_.CountTime() + search_timer_.CountTime();
    std::cout << "TEST LEVEL : " << VEND_LEVEL<<"\n"
              << "Cost Time : " << timer_.CountTime() << "\n"
              << "Search Time : " << search_timer_.CountTime() << "\n"
              << "Total Time : " << total_time << "\n"
              << "Triangle count : " << count_ << "\n";

    output << " TEST LEVEL " << ',' << VEND_LEVEL << ',' << "time " << timer_.CountTime() << ',' << " triangle count"
           << ',' << count_ << "," << "search time " << ',' << search_timer_.CountTime() << ',' << "total time" << ','
           << total_time << ',';
#else
    std::cout<<" Count By InterSection \n"
             <<" Cost Time : "<<timer_.CountTime() <<"Triangle count : "<< count_<<"\n";
    output<<','<< " InterSection"<<','<<"total time "<<timer_.CountTime()<<','<<" triangle count"<<','<<count_<<","
#endif


#if (VEND_LEVEL >= 1)
    std::cout << " edges filtered by vend:" << vend_message_.edge_filtered << "  total edges: "
              << vend_message_.total_edges << "\n";
    output << "edge filtered" << ',' << vend_message_.edge_filtered << ',' << "total edges " << ','
           << vend_message_.total_edges << ',';
#endif

#if (VEND_LEVEL >= 2)
    std::cout<< " Adj Filtered times : "<<vend_message_.adj_filtered<<"\n";
    output<<"adj filtered"<<','<<vend_message_.adj_filtered;
#endif
    output << "\n";
    output.close();
}


