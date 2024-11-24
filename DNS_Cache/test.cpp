#include "test.h"

void two_threads() {
    DNSCache& dnsCache = DNSCache::getInstance(3);

    auto update_dns = [&dnsCache]() {
        dnsCache.update("example.com", "93.184.215.14");
        dnsCache.update("google.com", "64.233.162.102");
        dnsCache.update("github.com", "140.82.121.4");
        };

    auto resolve_dns = [&dnsCache]() {
        std::cout << dnsCache.resolve("example.com") << std::endl;
        std::cout << dnsCache.resolve("google.com") << std::endl;
        std::cout << dnsCache.resolve("github.com") << std::endl;
        };

    std::thread t1(update_dns);
    std::thread t2(resolve_dns);

    t1.join();
    t2.join();

    assert(dnsCache.resolve("example.com") == "93.184.215.14");
    assert(dnsCache.resolve("google.com") == "64.233.162.102");
    assert(dnsCache.resolve("github.com") == "140.82.121.4");

    std::cout << "Test two_threads passed!" << std::endl;
}

void four_threads() {
    
    DNSCache& dnsCache = DNSCache::getInstance(3);

    auto update_dns = [&dnsCache]() {
        dnsCache.update("example.com", "93.184.215.14");
        dnsCache.update("google.com", "64.233.162.102");
        dnsCache.update("github.com", "140.82.121.4");
    };

    auto resolve_dns = [&dnsCache]() {
        dnsCache.resolve("example.com");
        dnsCache.resolve("google.com");
        dnsCache.resolve("github.com");
    };

    std::vector<std::thread> threads;
    threads.push_back(std::thread(update_dns));
    threads.push_back(std::thread(update_dns));
    threads.push_back(std::thread(resolve_dns));
    threads.push_back(std::thread(resolve_dns));

    for (auto& t : threads) {
        t.join();
    }

    assert(dnsCache.resolve("example.com") == "93.184.215.14");
    assert(dnsCache.resolve("google.com") == "64.233.162.102");
    assert(dnsCache.resolve("github.com") == "140.82.121.4");

    std::cout << "Test four_threads passed!" << std::endl;
}


void test_dns_cache_overflow() {
    DNSCache& dnsCache = DNSCache::getInstance(3);

    std::vector<std::string> domains = {
        "example.com", "google.com", "github.com", "openai.com", "yahoo.com", "microsoft.com", "apple.com",
        "amazon.com", "reddit.com", "stackoverflow.com", "youtube.com", "facebook.com", "twitter.com"
    };

    int ip_last_octet = 1;
    for (const auto& domain : domains) {
        std::string ip = "192.168.1." + std::to_string(ip_last_octet++);
        dnsCache.update(domain, ip);
    }

    // Проверяем, что кэш не переполнился, удалив старые записи
    assert(dnsCache.resolve("example.com").empty());  // Должно быть пустым, так как запись удалена
    assert(dnsCache.resolve("google.com").empty());
    assert(dnsCache.resolve("github.com").empty());
    assert(dnsCache.resolve("openai.com").empty());
    assert(dnsCache.resolve("yahoo.com").empty());
    assert(dnsCache.resolve("microsoft.com").empty());
    assert(dnsCache.resolve("apple.com").empty());
    assert(dnsCache.resolve("amazon.com").empty());
    assert(dnsCache.resolve("reddit.com").empty());
    assert(dnsCache.resolve("stackoverflow.com").empty());

    // Проверяем, что новые записи присутствуют и имеют корректный IP
    assert(dnsCache.resolve("youtube.com") == "192.168.1.11");
    assert(dnsCache.resolve("facebook.com") == "192.168.1.12");
    assert(dnsCache.resolve("twitter.com") == "192.168.1.13");

    std::cout << "Large data overflow test passed!" << std::endl;
}

void test_singleton() {
    DNSCache& instance1 = DNSCache::getInstance(3); // Размер устанавливается только при первом вызове
    DNSCache& instance2 = DNSCache::getInstance(4);
    DNSCache& instance3 = DNSCache::getInstance(3);
    DNSCache& instance4 = DNSCache::getInstance(6);

    // Проверка, что экземпляры указывают на один и тот же объект
    assert(&instance1 == &instance2);
    assert(&instance2 == &instance3);
    assert(&instance3 == &instance4);

    std::cout << "Singleton test passed!" << std::endl;
}

void test_dns_cache_performance() {
    auto& dnsCache = DNSCache::getInstance(10000);

    {
        LOG_DURATION("Massive update");
        for (int i = 0; i < 10000; ++i) {
            dnsCache.update("example" + std::to_string(i) + ".com", "192.168.0." + std::to_string(i % 255));
        }
    }

    {
        LOG_DURATION("Massive resolve");
        for (int i = 0; i < 10000; ++i) {
            dnsCache.resolve("example" + std::to_string(i) + ".com");
        }
    }

    {
        LOG_DURATION("Cache overflow handling");
        for (int i = 10000; i < 20000; ++i) {
            dnsCache.update("example" + std::to_string(i) + ".com", "10.0.0." + std::to_string(i % 255));
        }
    }
}

void test_dns_cache_performance_with_threads() {
    auto& dnsCache = DNSCache::getInstance(10000);

    auto update_dns = [&dnsCache]() {
        LOG_DURATION("DNS Update");
        for (int i = 0; i < 15000; ++i) {
            dnsCache.update("example" + std::to_string(i) + ".com", "192.168.0." + std::to_string(i % 255));
        }
        };

    auto resolve_dns = [&dnsCache]() {
        LOG_DURATION("DNS Resolve");
        for (int i = 0; i < 15000; ++i) {
            dnsCache.resolve("example" + std::to_string(i) + ".com");
        }
        };

    {
        LOG_DURATION("Total time for both update and resolve");
        std::vector<std::thread> threads;

        threads.push_back(std::thread(update_dns));
        threads.push_back(std::thread(resolve_dns));

        for (auto& t : threads) {
            t.join();
        }
    }
}



