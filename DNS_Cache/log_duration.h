#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)

class LogDuration {
public:
    using Clock = std::chrono::steady_clock;

    explicit LogDuration(const std::string& id)
        : id_(id), start_time_(Clock::now()) {}

    ~LogDuration() {
        using namespace std::chrono;
        const auto end_time = Clock::now();
        const auto duration = end_time - start_time_;
        const auto thread_id = std::this_thread::get_id();

        static std::mutex log_mutex;
        std::lock_guard<std::mutex> lock(log_mutex);

        std::cerr << "[" << thread_id << "] " << id_ << ": "
            << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
            << " ms" << std::endl;
    }

private:
    const std::string id_;
    const Clock::time_point start_time_;
};
