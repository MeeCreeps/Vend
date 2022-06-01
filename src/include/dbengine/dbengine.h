//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_DB_H
#define VEND_DB_H

#include <vector>
#include <string>
#include <iostream>
#include <memory>

class DbEngine {

public:
    DbEngine() {}

    DbEngine(const std::string &db_path) : db_path_(db_path) {}

    virtual ~DbEngine() {};

    virtual bool Open() = 0;

    virtual bool Close() {};

    /**
     *  get value from db engine
     *  @return   true: find the value
     *            false: key doesn't exist
     * */
    // edge format
    virtual bool Get(uint32_t key, uint32_t *value) = 0;

    // adjacency list format
    virtual bool Get(uint32_t key, std::vector<uint32_t> *value) = 0;

    virtual bool Get(uint32_t key, std::set<uint32_t> *value) = 0;

    /**
     *  update/insert key——value
     *  @return    true: success  false: fail
     * */

    virtual bool Put(uint32_t key, uint32_t value) = 0;

    virtual bool Put(uint32_t key, const std::vector<uint32_t> &value) = 0;

    // bath write , more efficient than put
    virtual void BatchWrite(const std::vector<uint32_t> &keys, const std::vector<std::vector<uint32_t>> &values) {};

    virtual void InitIter() {}

    // Scan database, get next key-vale
    virtual bool Next(uint32_t *key, std::vector<uint32_t> *value) {};
protected:
    std::string db_path_;

};

#endif //VEND_DB_H
