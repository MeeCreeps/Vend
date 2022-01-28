//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//



#include <iostream>
#include "graph/graph.h"


void Graph::Init() {
    // load or build encoding
    if (!db_path_.empty())
        graph_db_ = new RocksDb(db_path_);
    if (vend_type_ != VendType::NoVend) {
        if (access(vend_path_.c_str(), F_OK) != 0 && !data_path_.empty()) {
            // encode file doesn't exists ,load data
            LoadData();
            vend = VendFactory::GetEncode(vend_type_, adjacency_list, vend_path_, graph_db_);
            BuildEncode();

        } else {
            vend = VendFactory::GetEncode(vend_type_, adjacency_list, vend_path_, graph_db_);
            vend->LoadEncode();

        }
    }
    std::cout << "graph build finished" << std::endl;
}

void Graph::LoadData() {
    assert(!data_path_.empty());
    // id begins with 1
    adjacency_list.resize(VERTEX_SIZE + 1);
    std::ifstream infile(data_path_);
    std::string line;
    while (std::getline(infile, line)) {
        std::vector<uint32_t> temp;
        uint32_t vertex1, vertex2;
        std::istringstream line_string(line);
        line_string >> vertex1 >> vertex2;
        assert(vertex1 != vertex2);

        adjacency_list[vertex1].insert(vertex2);
        adjacency_list[vertex2].insert(vertex1);
    }
    infile.close();
}

void Graph::SaveData() {
    assert(!adjacency_list.empty());
    assert(graph_db_ != nullptr);
    for (uint32_t id = 1; id <= VERTEX_SIZE; ++id) {
        std::vector<uint32_t> neighbors(adjacency_list[id].begin(),adjacency_list[id].end());
        graph_db_->Put(id, neighbors);
    }
}

void Graph::BuildEncode() {
    vend->BuildEncoding();
    vend->EncodePersistent();

}

void Graph::DbInsert(const uint32_t &vertex1, const uint32_t &vertex2) {
    std::vector<uint32_t> neighbors;

    graph_db_->Get(vertex1, &neighbors);
    neighbors.push_back(vertex2);
    std::sort(neighbors.begin(), neighbors.end());
    graph_db_->Put(vertex1, neighbors);

    graph_db_->Get(vertex2, &neighbors);
    neighbors.push_back(vertex1);
    std::sort(neighbors.begin(), neighbors.end());
    graph_db_->Put(vertex2, neighbors);

}

void Graph::DbDelete(const uint32_t &vertex1, const uint32_t &vertex2) {

    std::vector<uint32_t> neighbors;

    graph_db_->Get(vertex1, &neighbors);
    auto iter = std::find(neighbors.begin(), neighbors.end(), vertex2);
    if (iter != neighbors.end()) {
        neighbors.erase(iter);
        graph_db_->Put(vertex1, neighbors);
    }


    std::vector<uint32_t>().swap(neighbors);

    graph_db_->Get(vertex2, &neighbors);
    iter = std::find(neighbors.begin(), neighbors.end(), vertex1);
    if (iter != neighbors.end()) {
        neighbors.erase(iter);
        graph_db_->Put(vertex2, neighbors);
    }


}

bool Graph::DbQuery(const uint32_t &vertex1, const uint32_t &vertex2) {
    std::vector<uint32_t> neighbors;
    graph_db_->Get(vertex1, &neighbors);
    auto iter = std::find(neighbors.begin(), neighbors.end(), vertex2);
    return iter != neighbors.end();
}


void Graph::BackUpDb() {
    std::uniform_int_distribution<unsigned> u;
    std::default_random_engine e;
    e.seed(time(NULL));
    std::string bak_path = db_path_ + VEND_STRING[vend_type_] + std::to_string(u(e));
    std::string command = "cp -r " + db_path_ + " " + bak_path;
    system(command.c_str());
    db_path_ = bak_path;

}

void Graph::DestoryDb() {
    if (graph_db_ != nullptr) {
        graph_db_->Close();
        graph_db_ = nullptr;
    }
    std::string command = "rm -rf " + db_path_;
    system(command.c_str());
    db_path_ = "";

}




