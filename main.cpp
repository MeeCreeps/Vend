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


#include "common/config.h"
#include "execution/build_execution.h"
#include "execution/insert_execution.h"
#include "execution/score_execution.h"
#include "execution/delete_execution.h"
#include "execution/query_execution.h"
#include "execution/random_query_execution.h"
#include "execution/random_score_execution.h"
#include "execution/full_score_execution.h"
#include "execution/triangle_execution.h"
#include "CLI11.hpp"
#include "easylog/easylogging++.h"
#include "util/utils.h"
INITIALIZE_EASYLOGGINGPP

#include <iostream>

int main(int argc, char **argv) {

    CLI::App app{"VEND"};
    std::string data_path, db_path, vend_prefix, output_path, pair_path_prefix;

    app.add_option("-d", data_path, " origin data which is stored in txt format ");
    app.add_option("-b", db_path, "database path for  data graph")->required();
    app.add_option("-v", vend_prefix, "prefix of path for encode")->required();
    app.add_option("-o", output_path, "output path ")->required();
    app.add_option("-p", pair_path_prefix, "query edge path")->required();

    bool is_build = false, is_insert = false, is_delete = false, is_triangle = false, is_walk_query = false, is_random_query = false,
            is_full_score = false, is_random_score = false, is_walk_score = false;

    app.add_option("--ib", is_build, "build  encode for vend");
    app.add_option("--ii", is_insert, "insert experiment");
    app.add_option("--id", is_delete, "delete experiment");
    app.add_option("--iwq", is_walk_query, "query (random walk) experiment");
    app.add_option("--irq", is_random_query, "query (randomly) experiment");
    app.add_option("--ifs", is_full_score, "full score experiment");
    app.add_option("--irs", is_random_score, "score (choose edges randomly) experiment");
    app.add_option("--iws", is_walk_score, "score (choose edges by random walk) experiment");
    app.add_option("--it", is_triangle, "triangle  experiment");
    CLI11_PARSE(app, argc, argv);


    std::string insert_pair_path = pair_path_prefix + "insert_pair.txt";
    std::string delete_pair_path = pair_path_prefix + "delete_pair.txt";
    std::string random_query_pair_path = pair_path_prefix + "random_query_pair.txt";
    std::string walk_query_pair_path = pair_path_prefix + "walk_query_pair.txt";
    std::string random_score_pair_path = pair_path_prefix + "random_score_pair.txt";
    std::string walk_score_pair_path = pair_path_prefix + "walk_score_pair.txt";
    std::string triangle_pair_path = pair_path_prefix + "triangle_pair.txt";

    VendType types[6] = {VendType::NoVend,
                         VendType::BloomFilterInt, VendType::HashBit, VendType::BloomFilterBit, VendType::Hybrid,
                         VendType::Range};
    int plan_nums = 6;

    if(VERTEX_SIZE>100000000)
        IS_LARGE=true;
    //build encode

    if (is_build) {
        plan_nums = 5;
        std::cout << "start building \n";
        for (int i = 0; i < plan_nums; ++i) {
            BuildExecution *build_execution = new BuildExecution(data_path, db_path, vend_prefix, types[i]);
            build_execution->Execute();
            std::cout << " graph " << VEND_STRING[types[i]] << " build finished " << std::endl;
            delete build_execution;
        }
        std::cout << "build finished \n";
    }


    // insert experiment
    if (is_insert) {
        std::cout << "start inserting \n";
        for (int i = 1; i < plan_nums; ++i) {
            InsertExecution *insert_execution = new InsertExecution(insert_pair_path, vend_prefix, output_path, db_path,
                                                                    types[i]);
            insert_execution->Execute();
            delete insert_execution;
        }
        std::cout << "insert experiment finished \n";
    }


    // delete experiment
    if (is_delete) {
        std::cout << "start deleting \n";
        for (int i = 1; i < plan_nums; ++i) {
            DeleteExecution *delete_execution = new DeleteExecution(delete_pair_path, vend_prefix, output_path, db_path,
                                                                    types[i]);
            delete_execution->Execute();
            delete delete_execution;
        }
        std::cout << "delete experiment finished \n";
    }

    // random query experiment , return query time
    if (is_random_query) {
        std::cout << "start random querying (random ) \n";
        for (int i = 0; i < plan_nums; ++i) {
            RandomQueryExecution *random_query_execution = new RandomQueryExecution(random_query_pair_path, vend_prefix,
                                                                                    output_path, db_path,
                                                                                    types[i]);
            random_query_execution->Execute();
            delete random_query_execution;
        }
        std::cout << "query time experiment finished \n";

    }

    if (is_walk_query) {
        // walk query experiment , return query time
        std::cout << "start querying ( walk ) \n";
        for (int i = 0; i < plan_nums; ++i) {
            QueryExecution *query_execution = new QueryExecution(walk_query_pair_path, vend_prefix, output_path,
                                                                 db_path,
                                                                 types[i]);
            query_execution->Execute();
            delete query_execution;
        }
        std::cout << "query time experiment finished \n";
    }
    if (is_random_score) {
        // random score
        std::cout << "start getting random scores\n";
        RandomScoreExecution *random_score_execution = new RandomScoreExecution(random_score_pair_path, vend_prefix,
                                                                                output_path,
                                                                                db_path);
        random_score_execution->Execute();
        delete random_score_execution;
        std::cout << "score experiment finished \n";
    }


    if (is_walk_score) {
        std::cout << "start getting scores (walk)\n";
        ScoreExecution *score_execution = new ScoreExecution(walk_score_pair_path, vend_prefix, output_path, db_path);
        score_execution->Execute();
        delete score_execution;
        std::cout << "score experiment finished \n";
    }

    if (is_full_score) {
        std::cout << "start getting scores\n";
        FullScoreExecution *full_execution = new FullScoreExecution(db_path, vend_prefix, output_path);
        full_execution->Execute();
        delete full_execution;
        std::cout << "score experiment finished \n";
    }

    //triangle count experiment
    if (is_triangle) {
        std::cout << "start querying triangle \n";
        std::unique_ptr<TriangleExecution> triangle = std::make_unique<TriangleExecution>(triangle_pair_path,
                                                                                          vend_prefix, output_path,
                                                                                          db_path);
        triangle->Execute();
        std::cout << "triangle experiment finished \n";
    }


    std::cout<<"memory usage:"<<memory::getMemory()<<" KB"<<std::endl;

}

#endif //MAIN
