//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_BUILD_EXECUTION_H
#define VEND_BUILD_EXECUTION_H

#include "execution/execution.h"

/**
 *  class to build encoding and database storage
 *
 * */
class BuildExecution : public Execution {
public:
    BuildExecution(std::string data_path, std::string db_path, std::string vend_prefix, VendType vend_type)
            : data_path_(data_path), Execution(db_path, vend_prefix, vend_type) {
        graph_ = new Graph(data_path, db_path, vend_path_, vend_type);
    }

    void Execute() override {
        graph_->Init();
        if (vend_type_ == VendType::NoVend) {
            graph_->LoadData();
            graph_->SaveData();
        }
    }


private:
    // csv txt file
    std::string data_path_;
};


#endif //VEND_BUILD_EXECUTION_H
