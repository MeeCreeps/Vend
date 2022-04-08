
//===----------------------------------------------------------------------===//
//
//
//
//
//
//
//
//===----------------------------------------------------------------------===//


#include "vend/range_vend.h"
#include <iostream>
#include <map>
#include "omp.h"

void RangeVend::BuildEncoding() {
    std::set<K_Node> k_core_order;
    uint32_t id = 1;
    std::map<uint32_t, std::set<K_Node>::iterator> location;
    // represent the time that a vertex was cut
    for (int id = 1; id < adjacency_list_.size(); ++id) {
        K_Node node;
        node.id = id;
        node.neighbors_num = adjacency_list_[id].size();
        auto iter = k_core_order.insert(node).first;
        location[node.id] = iter;
    }

    int i = 0;
    // k core
    while (!k_core_order.empty() && k_core_order.begin()->neighbors_num <= encodes_->GetIntSize()) {

        auto first = k_core_order.begin();
        std::string value;
        std::vector<uint32_t> neighbors;
        const int &vertex = first->id;
        //update neighbor
        for (auto &v:adjacency_list_[vertex]) {
            if (v == 0)
                continue;
            // update node
            K_Node node{};
            node.id = v;
            node.neighbors_num = location[v]->neighbors_num - 1;
            adjacency_list_[v].erase(vertex);
            // update location
            k_core_order.erase(location[v]);
            location[v] = k_core_order.insert(node).first;
            neighbors.push_back(v);

        }
        k_core_order.erase(location[vertex]);
        // encode
        encodes_->EncodeVertex(vertex, neighbors);
    }
    std::cout << "k core finished : kcore size:" << VERTEX_SIZE - k_core_order.size() << "\n";
    uint32_t edge_left=0;
    for(auto &v:adjacency_list_){
        edge_left+=v.size();
    }
    std::cout<<"edge left:"<<edge_left/2<<"\n";
    int index = 0;
    // encode left nodes
    std::vector<K_Node> nodes_left(k_core_order.begin(), k_core_order.end());

//    omp_set_num_threads(30);
//
//    #pragma omp parallel for
    for (int i = 0; i < nodes_left.size(); ++i) {
        uint32_t vertex = nodes_left[i].id;
        std::vector<uint32_t> neighbors;
        for (auto &neighbor:adjacency_list_[vertex])
            neighbors.push_back(neighbor);
        // encode
        encodes_->EncodeVertex(vertex, neighbors);
    }

    std::cout << " encode finished" << "\n";
}