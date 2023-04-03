#pragma once
#include <chrono>
#include <iostream>

#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x) 

class LogDuration{
public:
    using Clock = std::chrono::steady_clock;

    LogDuration(const std::string Op) : operation(Op)
    {}

    ~LogDuration(){
        using namespace std::literals;
        const auto end_time = Clock::now();
        std::cerr << operation << ": "s << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms" << std::endl;
    }
private:
    const Clock::time_point start_time = Clock::now();
    std::string operation = std::string("", 1);
};