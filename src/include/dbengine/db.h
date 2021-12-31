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

typedef std::string KeyType ;
typedef std::string ValueType;
class Db {

public:
    Db(const std::string &db_path) : db_path_(db_path) {}

    virtual bool Open() = 0;

    virtual bool Close() = 0;

    /**
     *  get value from db engine
     *  @return   true: find the value
     *            false: key doesn't exist
     * */

    virtual bool Get(const KeyType &key, ValueType *value) = 0;

    /**
     *  update/insert key——value
     *  @return    true: success  false: fail
     * */

    virtual bool Put(const KeyType &key, const ValueType &value) = 0;

    /**
     *  test whether key exists or not
     * */

    virtual bool TestKey(const KeyType &key) = 0;

private:
    std::string db_path_;

};

#endif //VEND_DB_H
