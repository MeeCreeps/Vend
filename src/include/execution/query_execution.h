//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_QUERY_EXECUTION_H
#define VEND_QUERY_EXECUTION_H

#include "pair_list.h"
#include "util/timer.h"
#include "algorithm"

class QueryExecution : public PairList, public Execution {

public:
    QueryExecution(std::string pair_path, std::string vend_prefix, std::string output_path, std::string db_path,
                   VendType vend_type)
            : PairList(pair_path, output_path, QUERY_LIST_SIZE), Execution(db_path, vend_prefix, vend_type) {
        graph_ = new Graph(db_path, vend_path_, vend_type);
    }

    void Execute() override {
        graph_->BackUpDb();
        graph_->Init();
        Init();
        Timer *timer = new Timer();
        int non_pair = 0, filter = 0;
        timer->StartTimer();
        for (auto &pair:pair_list_) {
            if (vend_type_ == VendType::NoVend || graph_->GetEdge(pair.first, pair.second) == PairType::Uncertain)
                graph_->DbQuery(pair.first, pair.second);


        }
        timer->StopTimer();
        std::ofstream output(output_path_, std::ios::out|std::ios::app);
        std::cout << VEND_STRING[vend_type_] << " query time cost :" << timer->CountTime() << "\n";
        output << VEND_STRING[vend_type_] << "," << "query " << "," << list_size_ << "," << timer->CountTime() << "\n";
        graph_->DestoryDb();

    };

    void CreateList(uint32_t vertex_size, std::vector<std::pair<uint32_t, uint32_t>> *vertex_list) override {
      std::vector<uint32_t> random_array;
      std::uniform_int_distribution<unsigned> u;
      std::default_random_engine e;
      e.seed(time(NULL));
      for (int i = 1; i <= VERTEX_SIZE; ++i)
          random_array.push_back(i);
    
      std::shuffle(random_array.begin(), random_array.end(), std::mt19937(std::random_device()()));
      uint32_t vertex1, vertex2;
        for (auto vertex:random_array) {
            std::vector<uint32_t> neighbors;
            graph_->GetNeighbors(vertex, &neighbors);
            int neighbor_size = neighbors.size();
            if (neighbor_size <= 1)
                continue;
            for (int i = 0; i < neighbor_size / 10 + 1; ++i) {
                int random1 = u(e) % neighbor_size;
                int random2 = u(e) % neighbor_size;
                if (random1 == random2)
                    continue;
                vertex_list->push_back({neighbors[random1], neighbors[random2]});
                if (vertex_list->size() >= vertex_size)
                    return;
            }
        }
     
    };


};

#endif //VEND_QUERY_EXECUTION_H
