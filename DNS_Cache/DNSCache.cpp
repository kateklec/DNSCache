#include "DNSCache.h"

DNSCache& DNSCache::getInstance(size_t max_size) {
    static DNSCache instance(max_size);
    return instance;
}

DNSCache::DNSCache(size_t max_size) : max_size_(max_size) {
    dns_map_.reserve(max_size);
}

void DNSCache::update(const std::string& name, const std::string& ip) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = dns_map_.find(name);

    if (it != dns_map_.end()) {
        it->second.ip = ip;
        domain_list_.splice(domain_list_.end(), domain_list_, it->second.pos_in_domain_list_);
    }
    else {
        if (dns_map_.size() == max_size_) {
            const auto* lru_name = domain_list_.front();
            auto node = dns_map_.extract(*lru_name);
            node.key() = name;
            node.mapped() = { ip, --domain_list_.end() };
            domain_list_.front() = &node.key();
            domain_list_.splice(domain_list_.end(), domain_list_, domain_list_.begin());
            dns_map_.insert(std::move(node));
        }
        else {          
            auto result = dns_map_.emplace(name, CacheData{ip, {}});
            const auto& key_ref = result.first->first;
            domain_list_.push_back(&key_ref);
            result.first->second.pos_in_domain_list_ = std::prev(domain_list_.end());
        }
    }
}

std::string DNSCache::resolve(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = dns_map_.find(name);
    if (it == dns_map_.end()) {
        return "";
    }
    domain_list_.splice(domain_list_.end(), domain_list_, it->second.pos_in_domain_list_);
    return it->second.ip;
}










