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

#include <string>
#include "graph/graph.h"


class Execution {

public:
    Execution(){}
    Execution(std::string db_path, std::string vend_prefix, VendType vend_type) : db_path_(db_path), vend_path_(
            VendFactory::GetVendPath(vend_prefix, vend_type)), vend_type_(vend_type) {}

    virtual void Execute() = 0;

    ~Execution(){
        delete graph_;
        graph_= nullptr;
    }
protected:
    std::string db_path_;
    std::string vend_path_;
    Graph *graph_= nullptr;
    VendType vend_type_;


};


#endif //VEND_EXECUTION_H
