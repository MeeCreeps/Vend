
#ifndef VEND_DGRAPH_H
#define VEND_DGRAPH_H


#include "dbengine.h"
#include <grpc++/grpc++.h>
#include "grpc/api.grpc.pb.h"
#include "util/json.cpp"

#include <iostream>
#include <memory>
#include <string>

using json = nlohmann::json;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using api::Request;
using api::Response;
using api::Dgraph;



class DGraph : public DbEngine {

public:
    DGraph(std::string url) : DbEngine(), rpc_url_(url),stub_(Dgraph::NewStub(grpc::CreateChannel(rpc_url_, grpc::InsecureChannelCredentials()))) {};

    bool Open() override {
        return true;
    };

    bool Get(uint32_t key, std::vector<uint32_t> *value) override {

    };

    bool QueryEdge(uint32_t vertex1, uint32_t vertex2) override {

        std::string graphql = "{ \n  get(func: uid(" + std::to_string(vertex1) +
                              ")) { \n   uid \n   connects @filter( uid( "
                              + std::to_string(vertex2) + "))\n     { \n    uid\n    }    \n     }  \n}\n";

        Request request;
        request.set_query(graphql);

        // Container for the data we expect from the server.
        Response reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->Query(&context, request, &reply);
        auto j = json::parse(reply.json());
        return j["get"].at(0).contains("connects");

    };

private:
    std::unique_ptr<Dgraph::Stub> stub_;
    std::string rpc_url_;
};


#endif //VEND_DGRAPH_H
