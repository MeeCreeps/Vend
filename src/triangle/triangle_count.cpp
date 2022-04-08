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
#include <omp.h>
/*
 *  @VEND_LEVEL
 *      0  :  Triangle count without vend filter
 *      1  ： use vend before binary search
 *
 * */

#define VEND_LEVEL 2
void TriangleCount::Init() {
    adj_db_ = std::make_shared<RocksDb>(adj_db_path_);
    encode_ = std::make_shared<HybridEncode>();
    encode_->LoadFromDb(encode_path_);

    // count degree
    degrees_.resize(VERTEX_SIZE + 1);
    adj_db_->InitIter();
    uint32_t vertex_id;
    std::vector<uint32_t> neighbors;
    while (adj_db_->Next(&vertex_id, &neighbors)) {
        int degree=neighbors.size();
        degrees_[vertex_id] = degree;
        max_degree_=std::max(max_degree_,degree);
    }

}
void TriangleCount::CountByAdj() {
    //omp_set_num_threads(THREAD_NUMS);


    timer_.StartTimer();
    //std::vector<time_message> node_time;
    HybridEncode::DecodeInfo v_neighbor_info[max_degree_];
    char neighbor_test[max_degree_];
    int v_degree,u_degree,n_degree;
    uint32_t u,n,v_neighbor_size;
    HybridEncode::DecodeInfo info_of_v;
    std::vector<uint32_t> neighbors_of_v,neighbors_of_u;

    for (uint32_t v = vertex_id_min_; v < vertex_id_upper_; ++v) {
        //Timer time;
        //time.StartTimer();
        adj_db_->Get(v, &neighbors_of_v);
        v_degree= degrees_[v];
        v_neighbor_size = neighbors_of_v.size();

#if (VEND_LEVEL >= 1)
        encode_->Decode(v,info_of_v);
        // decode neighbors of v
        //#pragma omp parallel for default(shared)
        for(int i=0;i<v_neighbor_size;++i){
            encode_->Decode(neighbors_of_v[i],v_neighbor_info[i]);
        }
#endif
        for(int i=0;i<v_neighbor_size;++i){
            u=neighbors_of_v[i];
            u_degree = degrees_[u];
                if (u_degree < v_degree || (u_degree == v_degree && u <= v))
                continue;


#if (VEND_LEVEL >= 2)
            // filter  adj by vend
            bool filtered=true;
            uint32_t triangles = 0;
            //#pragma omp parallel for reduction(+:triangles) shared(filtered) private(n,n_degree)
            int temp=0;
            for( ;temp<v_neighbor_size;++temp){
                n=neighbors_of_v[temp];
                n_degree = degrees_[n];
                if(n_degree>u_degree || (n_degree == u_degree && n>u)){
                    PairType type = encode_->NEpairTest(n,v_neighbor_info[temp],u,v_neighbor_info[i]);
                    if(type == PairType::Uncertain ){
                        neighbor_test[temp]=0x01;
                        filtered= false;
                        break;
                    }else if(type == PairType::Neighbor){
                        neighbor_test[temp]=0x02;
                        ++triangles;
                    }else{
                        neighbor_test[temp]=0x04;
                    }
                }
            }
            if(filtered){
                count_+=triangles;
                //++encode_message_.adj_filtered;
                continue;
            }
#endif
            //++encode_message_.total_adj;
            adj_db_->Get(u, &neighbors_of_u);


//#pragma omp parallel for reduction(+:count_)  private(n,n_degree)
            for(int j=0;j<v_neighbor_size;++j){
                n=neighbors_of_v[j];
                n_degree=degrees_[n];
                if (n_degree < u_degree || (n_degree == u_degree && n <= u))
                    continue;

#if (VEND_LEVEL>=2)
                //
                if(j<temp){
                    // neighbor
                    if(neighbor_test[j]&0x02){
                        ++count_;
                        continue;
                    }else if(neighbor_test[j]&0x04)
                        continue;
                }else {
                    PairType pair_type = encode_->NEpairTest(n,v_neighbor_info[j],u,v_neighbor_info[i]);
                    if(pair_type==PairType::Neighbor) {
                        ++count_;
                        continue;
                    }
                    else if(pair_type == PairType::NonNeighbor){
                        continue;
                    }
                }


#endif
#if (VEND_LEVEL == 1)

                PairType pair_type = encode_->NEpairTest(n,v_neighbor_info[j],u,v_neighbor_info[i]);
                if(pair_type==PairType::Neighbor) {
                    ++count_;
                //    ++encode_message_.edge_filtered;
                    continue;
                }
                else if(pair_type == PairType::NonNeighbor){
                //    ++encode_message_.edge_filtered;
                    continue;
                }
#endif
                if (BinarySearch(neighbors_of_u, n)) {
                    ++count_;
                }
            }
        }

        //time.StopTimer();
        //node_time.push_back({v,v_degree,time.CountTime()});
    }
    timer_.StopTimer();
    //OutTimer(node_time);
    OutputMessage();

}




void TriangleCount::CountByIntersection() {

    timer_.StartTimer();
    // id restrict :  v< u < intersection
    HybridEncode::DecodeInfo v_neighbor_info[max_degree_];
    HybridEncode::DecodeInfo info_of_v,info_of_u;
    int v_degree,u_degree,n_degree;
    std::vector<uint32_t> neighbors_of_u,neighbors_of_v;
    uint32_t u,n,v_neighbor_size,u_neighbor_size;
    for (uint32_t v = vertex_id_min_; v < vertex_id_upper_; ++v) {
        adj_db_->Get(v, &neighbors_of_v);
        v_neighbor_size = neighbors_of_v.size();

#ifdef VEND_LEVEL
        //decode first
        encode_->Decode(v,info_of_v);
        for(int i=0;i<v_neighbor_size;++i){
            encode_->Decode(neighbors_of_v[i],v_neighbor_info[i]);
        }
#endif


        for(int i=0;i<neighbors_of_v.size();++i){
            u=neighbors_of_v[i];
            if (u <= v)
                continue;

#ifdef VEND_LEVEL
            //adj filtered
            uint32_t triangles = 0;
            bool filtered=true;
            for(int j=0;j<neighbors_of_v.size();++j){
                n=neighbors_of_v[j];
                if(n>u){
                    PairType type = encode_->NEpairTest(u,v_neighbor_info[i],n,v_neighbor_info[j]);
                    if(type == PairType::Uncertain ){
                        filtered= false;
                        break;
                    }else if(type == PairType::Neighbor){
                        ++triangles;
                    }
                }
            }
            //++encode_message_.total_adj;
            if(filtered){
                count_+=triangles;
                //++encode_message_.adj_filtered;
                continue;
            }
#endif

            adj_db_->Get(u, &neighbors_of_u);
            count_ += InterSection(neighbors_of_u, neighbors_of_v, u);
        }
    }
    timer_.StopTimer();
    OutputMessage();

}

//void TriangleCount::CountByIntersection() {
//
//    timer_.StartTimer();
//    // id restrict :  v< u < intersection
//    HybridEncode::DecodeInfo v_neighbor_info[max_degree_];
//    int v_degree,u_degree,n_degree;
//    uint32_t u,n,v_neighbor_size,u_neighbor_size;
//    HybridEncode::DecodeInfo info_of_v,info_of_u,info_of_n;
//    std::vector<uint32_t> neighbors_of_v,neighbors_of_u;
//
//    for (uint32_t v = vertex_id_min_; v < vertex_id_upper_; ++v) {
//        adj_db_->Get(v, &neighbors_of_v);
//        v_degree= degrees_[v];
//        v_neighbor_size = neighbors_of_v.size();
//
//#ifdef VEND_LEVEL
//        //decode first
//        encode_->Decode(v,info_of_v);
//        for(int i=0;i<v_neighbor_size;++i){
//            encode_->Decode(neighbors_of_v[i],v_neighbor_info[i]);
//        }
//#endif
//
//
//        for(int i=0;i<v_neighbor_size;++i){
//            u=neighbors_of_v[i];
//            u_degree = degrees_[u];
//            if (u_degree < v_degree || (u_degree == v_degree && u <= v))
//                continue;
//
//#ifdef VEND_LEVEL
//            //adj filtered
//            uint32_t triangles = 0;
//            bool filtered=true;
//            for(int j=0;j<v_neighbor_size;++j){
//                n=neighbors_of_v[j];
//                n_degree=degrees_[n];
//                if(n_degree>u_degree || (n_degree == u_degree && n>u)){
//                    PairType type = encode_->NEpairTest(u,v_neighbor_info[i],n,v_neighbor_info[j]);
//                    if(type == PairType::Uncertain ){
//                        filtered= false;
//                        break;
//                    }else if(type == PairType::Neighbor){
//                        ++triangles;
//                    }
//                }
//            }
//            ++encode_message_.total_adj;
//            if(filtered){
//                count_+=triangles;
//                ++encode_message_.adj_filtered;
//                continue;
//            }
//#endif
//            adj_db_->Get(u, &neighbors_of_u);
//            count_ += InterSection(neighbors_of_u, neighbors_of_v, u);
//        }
//    }
//    timer_.StopTimer();
//    OutputMessage();
//
//}

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
    while (v_idx<v_size&&v_neighbors[v_idx] <= min)
        ++v_idx;
    while (u_idx<u_size&&u_neighbors[u_idx] <= min)
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
    std::cout << "TEST LEVEL : " << VEND_LEVEL<<"\n"
              << "Cost Time : " << timer_.CountTime()/1000000 << "\n"
              << "Triangle count : " << count_ << "\n";

    output << " TEST LEVEL " << ',' << VEND_LEVEL << ',' << "time " << timer_.CountTime() << ',' << " triangle count"
           << ',' << count_ << "," << "search time "<< ',' << "total time";
#else
    std::cout<<" Count By InterSection \n"
             <<" Cost Time : "<<timer_.CountTime() <<"Triangle count : "<< count_<<"\n";
    output<<','<< " InterSection"<<','<<"total time "<<timer_.CountTime()<<','<<" triangle count"<<','<<count_<<",";
#endif


#if (VEND_LEVEL >= 1)
    std::cout << " edges filtered by vend:" << encode_message_.edge_filtered << "  total edges: "
              << encode_message_.total_edges << "\n";
    output << "edge filtered" << ',' << encode_message_.edge_filtered << ',' << "total edges " << ','
           << encode_message_.total_edges << ',';
#endif

#if (VEND_LEVEL >= 2 || VEND_LEVEL==-1)
    std::cout<< " Adj Filtered times : "<<encode_message_.adj_filtered<<"\n"
            << "total adj :"<<encode_message_.total_adj<<"\n";
    output<<"adj filtered"<<','<<encode_message_.adj_filtered<<','<<"total adj"<<','<<encode_message_.total_adj;
#endif
    output << "\n";
    output.close();
}

void TriangleCount::OutTimer(std::vector<time_message> &node_time) {
    std::ofstream output(output_path_, std::ios::app | std::ios::out);
    for(auto iter=node_time.begin();iter!=node_time.end();++iter){
        output<<"id"<<','<<iter->id<<','<<"degree"<<','<<iter->degree<<','<<"time"<<','<<iter->time<<std::endl;
    }
    output.close();
}


