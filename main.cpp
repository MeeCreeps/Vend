//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//
#ifndef MAIN
#define MAIN

#include "util/bitset.h"
#include "common/config.h"
#include "execution/build_execution.h"
#include "execution/insert_execution.h"
#include "execution/score_execution.h"
#include "execution/delete_execution.h"
#include "execution/query_execution.h"
#include "execution/random_query_execution.h"
#include "execution/random_score_execution.h"
#include "execution/full_score_execution.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "dbengine/rocksdb.h"
#include "cmath"


int main(int argc, char **argv) {


    std::string data_path = argv[1];
    std::string db_path = argv[2];
    std::string vend_prefix = argv[3];
    std::string output_path = argv[4];
    std::string pair_path_prefix = argv[5];

    std::string insert_pair_path = pair_path_prefix + "insert_pair.txt";
    std::string delete_pair_path = pair_path_prefix + "delete_pair.txt";
    std::string query_pair_path = pair_path_prefix + "query_pair.txt";
    std::string score_pair_path = pair_path_prefix + "score_pair.txt";
    VendType types[6] = {VendType::NoVend,  VendType::Hybrid, VendType::Range,VendType::HashBit,
                         VendType::BloomFilterInt, VendType::BloomFilterBit};
    int plan_nums = 6;
    //build encode
//    std::cout << "start building \n";
//    for (int i =0; i <plan_nums; ++i) {
//        BuildExecution *build_execution = new BuildExecution(data_path, db_path, vend_prefix,types[i]);
//        build_execution->Execute();
//        std::cout<<" graph "<<VEND_STRING[types[i]]<<" build finished "<<std::endl;
//        delete build_execution;
//    }
//    std::cout << "build finished \n";

//    std::cout << "start deleting \n";
//    for (int i = 1; i < plan_nums; ++i) {
//        DeleteExecution *delete_execution = new DeleteExecution(delete_pair_path, vend_prefix, output_path, db_path,
//                                                                types[i]);
//        delete_execution->Execute();
//        delete delete_execution;
//    }
//    std::cout << "delete experiment finished \n";
//
//    // insert experiment
//    std::cout << "start inserting \n";
//    for (int i = 1; i < plan_nums; ++i) {
//        InsertExecution *insert_execution = new InsertExecution(insert_pair_path, vend_prefix, output_path, db_path,
//                                                                types[i]);
//        insert_execution->Execute();
//        delete insert_execution;
//    }
//    std::cout << "insert experiment finished \n";
//
//   // query experiment , return query time
//    std::cout << "start querying \n";
//    for (int i = 0; i < plan_nums; ++i) {
//        QueryExecution *query_execution = new QueryExecution(query_pair_path, vend_prefix, output_path, db_path,
//                                                             types[i]);
//        query_execution->Execute();
//        delete query_execution;
//    }
//    std::cout << "query time experiment finished \n";
//
//    // query experiment , return query time
//    std::cout << "start random querying \n";
//    for (int i = 0; i < plan_nums; ++i) {
//        RandomQueryExecution *random_query_execution = new RandomQueryExecution(query_pair_path, vend_prefix, output_path, db_path,
//                                                             types[i]);
//        random_query_execution->Execute();
//        delete random_query_execution;
//    }
//    std::cout << "query time experiment finished \n";
////
//    // random score
//    std::cout << "start getting random scores\n";
//    RandomScoreExecution *random_score_execution = new RandomScoreExecution(score_pair_path,vend_prefix, output_path,db_path);
//    random_score_execution->Execute();
//    delete random_score_execution;
//    std::cout << "score experiment finished \n";
//
//    // query experiment and return score
//    std::cout << "start getting scores\n";
//    ScoreExecution *score_execution = new ScoreExecution(score_pair_path,vend_prefix, output_path,db_path);
//    score_execution->Execute();
//    delete score_execution;
//    std::cout << "score experiment finished \n";
    // query experiment and return score
    std::cout << "start getting scores\n";
    FullScoreExecution *full_execution = new FullScoreExecution(vend_prefix, output_path);
    full_execution->Execute();
    delete full_execution;
    std::cout << "score experiment finished \n";
}

#endif //MAIN
