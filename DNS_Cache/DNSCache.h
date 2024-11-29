#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <mutex>
#include "custom_hash.h"

class DNSCache {
public:
    static DNSCache& getInstance(size_t max_size);

    void update(const std::string& name, const std::string& ip);
    std::string resolve(const std::string& name);

private:
    explicit DNSCache(size_t max_size);

    DNSCache(const DNSCache&) = delete;
    DNSCache& operator=(const DNSCache&) = delete;
    DNSCache(DNSCache&&) = delete;
    DNSCache& operator=(DNSCache&&) = delete;

    struct CacheData {
        std::string ip;
        std::list<const std::string*>::iterator pos_in_domain_list_;
    };

    size_t max_size_;
    std::unordered_map<std::string, CacheData, custom_hash> dns_map_;
    std::list<const std::string*> domain_list_;

    std::mutex mutex_;
};








