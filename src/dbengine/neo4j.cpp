
#include "dbengine/neo4j.h"

void Neo4j::GetNeighbors(uint32_t vertex, std::vector<uint32_t> &neighbors) {
    assert(connection_ != NULL);
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "MATCH (a)-[:connect]->(b) WHERE a.id ='%d' RETURN b", vertex);
    neo4j_result_stream_t *results = neo4j_run(connection_, buf, neo4j_null);
    neo4j_result_t *result = neo4j_fetch_next(results);
    while(result!=NULL){
        neo4j_value_t value = neo4j_result_field(result, 0);
        char buf[128];
        neighbors.push_back(std::stoi(neo4j_tostring(value, buf, sizeof(buf))));
        printf("%s\n", neo4j_tostring(value, buf, sizeof(buf)));
        result= neo4j_fetch_next(results);
    }
    neo4j_release(result);
    neo4j_close_results(results);
}

void Neo4j::SetNode(uint32_t min_id, uint32_t max_id) {
    assert(connection_ != NULL);
    char buf[64];
    for (uint32_t i = min_id; i <= max_id; ++i) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "CREATE(n {id:'%d'})", i);
        neo4j_result_stream_t *results = neo4j_run(connection_, buf, neo4j_null);
        if (results == NULL)
        {
            neo4j_perror(stderr, errno, "Failed to run statement");
            return ;
        }
        neo4j_close_results(results);
    }
}

void Neo4j::SetRelation(uint32_t vertex1, uint32_t vertex2) {
    assert(connection_ != NULL);
    char buf[128];
    memset(buf, 0, sizeof(buf));
    sprintf(buf,
            "MATCH (a),(b)\nWHERE a.id = '%d' AND b.id = '%d'\nCREATE (a)-[r:connect] -> (b)",
            vertex1, vertex2);
    neo4j_result_stream_t *results = neo4j_run(connection_, buf, neo4j_null);
    neo4j_close_results(results);
}

bool Neo4j::QueryEdge(uint32_t vertex1, uint32_t vertex2) {
    assert(connection_ != NULL);
    char buf[128];
    memset(buf, 0, sizeof(buf));
    sprintf(buf,
            "MATCH (a)-[r:RELTYPE] -> (b)\nWHERE a.id = '%d' AND b.id = '%d'\n",
            vertex1, vertex2);
    neo4j_result_stream_t *results = neo4j_run(connection_, buf, neo4j_null);
    neo4j_result_t *result = neo4j_fetch_next(results);
    if(result==NULL){
        neo4j_close_results(results);
        return false;
    }else{
        neo4j_release(result);
        neo4j_close_results(results);
        return true;
    }
}
