//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_ENCODEFACTORY_H
#define VEND_ENCODEFACTORY_H

#include "vend/bit_hash_vend.h"
#include "vend/bf_int_vend.h"
#include "vend/bf_bit_vend.h"
#include "vend/range_vend.h"
#include "vend/hybrid_vend.h"
#include "vend.h"
#include <map>

enum class VendType {
    BloomFilterInt, BloomFilterBit, Range, HashBit, Hybrid, NoVend
};

static std::map<VendType, std::string> VEND_STRING = {{VendType::BloomFilterBit, "BloomFilterBit"},
                                                      {VendType::BloomFilterInt, "BloomFilterInt"},
                                                      {VendType::Range,          "Range"},
                                                      {VendType::HashBit,        "HashBit"},
                                                      {VendType::Hybrid,         "Hybrid"},
                                                      {VendType::NoVend,         "NoVend"}};

class VendFactory {
public:
    /**
     *  choose encode method by vend type
     *
     *
     * */
    static void
    GetEncode(VendType vend_type, std::shared_ptr<std::vector<std::vector<uint32_t >>> &adj_list,
              const std::string &encode_path,
              std::shared_ptr<DbEngine> &db, std::shared_ptr<Vend> &vend) {

        switch (vend_type) {
            case VendType::BloomFilterInt:
                vend = std::make_shared<BFilterIntVend>(adj_list, encode_path, db);
                break;
            case VendType::BloomFilterBit:
                vend = std::make_shared<BFilterBitVend>(adj_list, encode_path, db);
                break;
            case VendType::HashBit:
                vend = std::make_shared<BitHashVend>(adj_list, encode_path, db);
                break;
            case VendType::Range:
                vend = std::make_shared<RangeVend>(adj_list, encode_path, db);
                break;
            case VendType::Hybrid:
                vend = std::make_shared<HybridVend>(adj_list, encode_path, db);
                break;
            case VendType::NoVend:
                vend = nullptr;
        }

    };

    /*
     *  for large graph
     * */
    static void
    GetEncode(VendType vend_type, const std::string &encode_path,
              std::shared_ptr<DbEngine> &db, std::shared_ptr<Vend> &vend) {

        switch (vend_type) {
            case VendType::BloomFilterInt:
                vend = std::make_shared<BFilterIntVend>(encode_path, db);
                break;
            case VendType::BloomFilterBit:
                vend = std::make_shared<BFilterBitVend>(encode_path, db);
                break;
            case VendType::HashBit:
                vend = std::make_shared<BitHashVend>(encode_path, db);
                break;
            case VendType::Range:
                vend = std::make_shared<RangeVend>(encode_path, db);
                break;
            case VendType::Hybrid:
                vend = std::make_shared<HybridVend>(encode_path, db);
                break;
            case VendType::NoVend:
                vend = nullptr;
        }

    };

    static std::string GetVendPath(std::string vend_prefix, VendType vend_type) {
        std::string vend_path;
        switch (vend_type) {
            case VendType::NoVend:
                return "";
            default:
                return vend_prefix + VEND_STRING[vend_type] + ".txt";
        }
    }

    static std::string GetOutputPath(std::string output_prefix, VendType vend_type) {
        std::string vend_path;
        switch (vend_type) {
            case VendType::NoVend:
                return "";
            default:
                return output_prefix + VEND_STRING[vend_type] + ".csv";
        }
    };
};

#endif //VEND_ENCODEFACTORY_H
