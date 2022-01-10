//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_EXECUTION_H
#define VEND_EXECUTION_H

#include "graph/graph.h"
#include <string>
#include <vector>
#include <unordered_map>
class Execution{

public:

    Execution(std::string db_path, std::string data_path = nullptr){}

    /**
     * construct graph and vend
     * */
    void Init();


    void Query();

    void Insert();

    void Delete();

    void CreateQueryList(std::vector<int> query_size,int repeat_times);

    std::set<uint32_t> LoadQueryList();

    void CreateInsertList(std::vector<int> insert_size,int repeat_times);

    std::unordered_map<uint32_t,uint32_t> LoadInsertList();

    void CreateDeleteList(std::vector<int> insert_size,int repeat_times);

    std::unordered_map<uint32_t,uint32_t> LoadDeleteList();

private:
    Graph *graph_;
    std::string vend_path_;
    std::string data_path_;

};





#endif //VEND_EXECUTION_H
