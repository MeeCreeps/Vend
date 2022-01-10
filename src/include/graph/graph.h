//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_GRAPH_H
#define VEND_GRAPH_H

#include "vend/vend.h"
#include "dbengine/db.h"

class Graph {
public:
    Graph(std::string vend_path, std::string data_path = nullptr) : vend_path_(vend_path), data_path_(data_path);

    /**
     *  read origin data from csv/txt etc. file  and initial adjacency list、edge database
     * */
    void LoadData();

    void BuildDb();

    void AddEdge(const uint32_t &vertex1, const uint32_t &vertex2);

    bool GetEdge(const uint32_t &vertex1, const uint32_t &vertex2);

    void RemoveEdge(const uint32_t &vertex1, const uint32_t &vertex2);

private:
    std::string data_path_;
    std::string vend_path_;
    uint32_t vertex_size = VERTEX_SIZE;
    std::vector<std::vector<uint32_t >> adjacency_list;
    Db *edge_db_;
    Db *encode_db_;
    Vend *vend;
};

#endif //VEND_GRAPH_H
