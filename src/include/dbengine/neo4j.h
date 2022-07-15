
#ifndef VEND_NEO4J_H
#define VEND_NEO4J_H

#include "neo4j-client.h"
#include "dbengine.h"
#include <vector>
#include <memory>
#include <string>
#include <cassert>
#include <iostream>
#include "common/config.h"
class Neo4j : public DbEngine {
public:
    Neo4j(std::string db_url) :DbEngine(), db_url_(db_url) {

        config_ = neo4j_new_config();
        neo4j_config_set_username(config_,USER_NAME.c_str() );
        neo4j_config_set_password(config_, PASSWORD.c_str());
        connection_ = neo4j_connect(db_url.c_str(), config_,NEO4J_NO_URI_PASSWORD);
        if (connection_ == NULL) {
            neo4j_perror(stderr, errno, "Connection failed");
        }
    };

    ~Neo4j() {
        Close();
    }

    bool Open() override { return connection_ != NULL; }


    bool Get(uint32_t vertex, std::vector<uint32_t> *neighbors) override;


    bool Put(uint32_t vertex1, uint32_t vertex2) override;

    bool QueryEdge(uint32_t vertex1, uint32_t vertex2) override;

    bool Close() override {
        if(config_!=NULL)
            neo4j_config_free(config_);
        if(connection_!=NULL)
            neo4j_close(connection_);
        neo4j_client_cleanup();
        return true;
    }

private:
    std::string db_url_;
    neo4j_connection_t *connection_;
    neo4j_config_t *config_;
};


#endif //VEND_NEO4J_H
