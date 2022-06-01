
#ifndef VEND_NEO4J_H
#define VEND_NEO4J_H

#include "neo4j-client.h"
#include <vector>
#include <memory>
#include <string>
#include <cassert>
#include <iostream>
class Neo4j {
public:
    Neo4j(std::string db_url) : db_url_(db_url) {
        config_ = neo4j_new_config();
        neo4j_config_set_username(config_, "neo4j");
        neo4j_config_set_password(config_, "zhy624me");
        connection_ = neo4j_connect(db_url.c_str(), config_, NEO4J_NO_URI_PASSWORD);
        if (connection_ == NULL) {
            neo4j_perror(stderr, errno, "Connection failed");

        }
    };

    ~Neo4j() {
        Close();
    }

    void GetNeighbors(uint32_t vertex, std::vector<uint32_t> &neighbors);

    void SetNode(uint32_t min_id, uint32_t max_id);

    void SetRelation(uint32_t vertex1, uint32_t vertex2);

    bool QueryEdge(uint32_t vertex1, uint32_t vertex2);

    void Close() {
        neo4j_config_free(config_);
        neo4j_close(connection_);
        neo4j_client_cleanup();
    }

private:
    std::string db_url_;
    neo4j_connection_t *connection_;
    neo4j_config_t *config_;
};


#endif //VEND_NEO4J_H
