

#ifndef VEND_TRIANGLE_EXECUTION_H
#define VEND_TRIANGLE_EXECUTION_H


#include "pair_list.h"
#include "util/timer.h"
#include "algorithm"
#include <map>


static constexpr int TRIANGLE_NODE_SIZE = 10000;

class TriangleExecution : public PairList {

public:
    TriangleExecution(std::string pair_path, std::string vend_prefix, std::string output_path, std::string db_path)
            : PairList(pair_path + ".triangle", output_path + ".triangle", TRIANGLE_NODE_SIZE)
    {

        db_ = std::make_unique<RocksDb>(db_path);
        for (int i = 0; i < PLAN_NUMS; ++i) {
            switch (Vends[i]) {
                case VendType::Hybrid:
                    encode_[i] = std::make_unique<HybridEncode>();
                    break;
                case VendType::Range:
                    encode_[i] = std::make_unique<RangeEncode>();
                    break;
                case VendType::HashBit:
                    encode_[i] = std::make_unique<BitHashEncode>();
                    break;
                case VendType::BloomFilterBit:
                    encode_[i] = std::make_unique<BFilterBitEncode>();
                    break;
                case VendType::BloomFilterInt:
                    encode_[i] = std::make_unique<BFilterIntEncode>();
                    break;
            }
            encode_[i]->LoadFromDb(VendFactory::GetVendPath(vend_prefix, Vends[i]));
            std::cout << "build " << VEND_STRING[Vends[i]] << " successfully" << std::endl;
        }
    }

    void InitDegree() {
        degrees_.resize(VERTEX_SIZE + 1);
        db_->InitIter();
        uint32_t vertex_id;
        std::vector<uint32_t> neighbors;
        while (db_->Next(&vertex_id, &neighbors)) {
            int degree = neighbors.size();
            degrees_[vertex_id] = degree;
            max_degree_ = std::max(max_degree_, degree);
        }
        std::cout << "degree finished" << max_degree_ << std::endl;
    }

    void Execute() override {
        //graph_->BackUpDb();
        //graph_->Init();
        Init();
        //InitDegree();
        Timer timer;
        timer.StartTimer();;
        CountTriangle();
        timer.StopTimer();
        OutPutTime();
        //std::ofstream output(output_path_, std::ios::out|std::ios::app);
//        std::cout << VEND_STRING[vend_type_] << " query time cost :" << timer->CountTime() / 1000000
//                  << " edge filtered: "
//                  << encode_message_.edge_filtered << " total edge:" << encode_message_.total_edges << " percentage"
//                  << (double) encode_message_.edge_filtered / encode_message_.total_edges * 100 << "\n";
//        output << VEND_STRING[vend_type_] << "," << "query " << "," << list_size_ << "," << timer->CountTime() / 1000000
//               << "\n";
        //graph_->DestoryDb();

    };

    void CountTriangle() {
        //std::vector<time_message> node_time;
        filter_edges=std::vector<uint32_t>(PLAN_NUMS,0);
        filter_time=std::vector<double>(PLAN_NUMS,0);
        uint32_t v, u, n, v_neighbor_size, u_neighbor_size;
        std::vector<uint32_t> neighbors_of_v, neighbors_of_u;
        std::cout << "start triangle" << std::endl;
        for (int idv = 0; idv < pair_list_.size(); ++idv) {
            Timer t;
            t.StartTimer();
            v = pair_list_[idv].first;
            db_->Get(v, &neighbors_of_v);
            v_neighbor_size = neighbors_of_v.size();

            for (int i = 0; i < v_neighbor_size; ++i) {
                u = neighbors_of_v[i];
                if (u <= v)
                    continue;
                db_->Get(u, &neighbors_of_u);
                for (int j = 0; j < v_neighbor_size; ++j) {
                    n = neighbors_of_v[j];
                    if (n <= u)
                        continue;
                    ++encode_message_.total_edges;
                    for (int k = 0; k < neighbors_of_u.size(); ++k) {
                        if (neighbors_of_u[k] == n) {
                            ++count_;
                            break;
                        }
                    }
                    t.StopTimer();
                    time_[idv][i][j] = t.CountTime() / 1000000;
                    for (int idp = 0; idp < PLAN_NUMS; ++idp) {
                        if (encode_[idp]->NEpairTest(u, n) != Uncertain) {
                            filter_time[idp] += time_[idv][i][j];
                            ++filter_edges[idp];
                        }
                    }

                    t.ResetTime();
                    t.StartTimer();
                }
            }
        }
    }

    void OutPutTime() {
        double total_time = 0.0;
        for (auto t1: time_) {
            for (auto t2: t1.second)
                for (auto t3: t2.second)
                    total_time += t3.second;
        }

        std::cout << " total :" << total_time << std::endl;

        // filtered by vend
        std::cout << "Plan NoVend   time:" << total_time << std::endl;
        for (int i = 0; i < PLAN_NUMS; ++i) {
            std::cout << "Paln " << VEND_STRING[Vends[i]] << "  filter time:" << filter_time[i] << " left time:"
                      << total_time - filter_time[i] << " time percentage:" << filter_time[i] / total_time * 100
                      << "% filter edge" << filter_edges[i] << " total edges: "
                      << encode_message_.total_edges << " edge percentage: "
                      << (double) filter_edges[i] / encode_message_.total_edges * 100 << "%\n";
        }
    }

    void CreateList(uint32_t vertex_size, std::vector<std::pair<uint32_t, uint32_t>> *vertex_list) override {
        std::vector<uint32_t> random_array;
        std::uniform_int_distribution<unsigned> u;
        std::default_random_engine e;
        e.seed(time(NULL));
        for (int i = 1; i <= VERTEX_SIZE; ++i)
            random_array.push_back(i);
        std::shuffle(random_array.begin(), random_array.end(), std::mt19937(std::random_device()()));
        for (int i = 0; i < TRIANGLE_NODE_SIZE; ++i)
            vertex_list->push_back({random_array[i], random_array[i]});

    };

private:
    std::unique_ptr<Encode> encode_[PLAN_NUMS];
    std::unique_ptr<DbEngine> db_;

    struct VendMessage {
        uint64_t adj_filtered = 0;
        uint64_t total_adj = 0;
        uint64_t edge_filtered = 0;
        uint64_t total_edges = 0;

        VendMessage operator+=(const VendMessage &rhs) {
            total_edges += rhs.total_edges;
            edge_filtered += rhs.edge_filtered;
            total_edges += rhs.total_edges;
        }

    };

    std::map<uint32_t, std::map<uint32_t, std::map<uint32_t, double>>> time_;
    int max_degree_ = 0;
    std::vector<int> degrees_;
    std::vector<double> filter_time;
    std::vector<uint32_t> filter_edges;
    Timer timer_;
    uint64_t count_;
    VendMessage encode_message_;

};


#endif //VEND_TRIANGLE_EXECUTION_H
