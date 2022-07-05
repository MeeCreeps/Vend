#include "graph/large_graph.h"


void LargeGraph::LoadAndSaveData() {
    assert(!data_path_.empty());

    std::ifstream infile(data_path_);
    std::string line;
    std::istringstream line_string;

    std::unordered_map<uint32_t, std::vector<uint32_t>> adj;
    int vertex_size = 0;
    uint32_t vertex1, vertex2, vertex_before = 0;
    // original data file is organized by first vertex id
    while (std::getline(infile, line)) {

        line_string.str(line);
        line_string >> vertex1 >> vertex2;

        if (vertex1 != vertex_before) {
            vertex_size++;
            vertex_before = vertex1;
        }
        // write for each batch
        if (vertex_size % write_batch == 0) {
            vertex_size = 0;
            UpdateEdges(adj);
        }
        // update edges
        adj[vertex1].push_back(vertex2);
        adj[vertex2].push_back(vertex1);

        line_string.clear();
    }
    UpdateEdges(adj);
}

void LargeGraph::UpdateEdges(std::unordered_map<uint32_t, std::vector<uint32_t>> &adj) {

    std::vector<uint32_t> keys;
    std::vector<std::vector<uint32_t>> values(adj.size());
    for (auto iter = adj.begin(); iter != adj.end(); ++iter) {
        std::vector<uint32_t> *original_adj = &values[keys.size()];
        if (graph_db_->Get(iter->first, original_adj)) {
            original_adj->insert(original_adj->end(), iter->second.begin(), iter->second.end());
        }
        keys.push_back(iter->first);
        graph_db_->BatchWrite(keys, values);
    }
    adj.clear();
}

void LargeGraph::Init() {

    if (!db_path_.empty())
        graph_db_ = std::make_shared<RocksDb>(db_path_);
    if (vend_type_ != VendType::NoVend) {
        if (access(vend_path_.c_str(), F_OK) != 0 && !data_path_.empty()) {
            // encode file when is not exists ,load data
            LoadData();
            VendFactory::GetEncode(vend_type_, vend_path_, graph_db_,vend);
            BuildEncode();

        } else {
            VendFactory::GetEncode(vend_type_, vend_path_, graph_db_,vend);
            vend->LoadEncode();

        }
    }
    std::cout << "graph initial finished" << std::endl;
}



