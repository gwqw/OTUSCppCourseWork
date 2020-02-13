#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <condition_variable>

#include "task_generator.h"
#include "thread_pool.h"
#include "project_config.h"

class ISubscriber {
public:
    virtual ~ISubscriber() = default;
    virtual void update(ResultHolder calc_result) = 0;
};

using SubscriberHolder = std::unique_ptr<ISubscriber>;

class ResultSaver : public ISubscriber {
public:
    explicit ResultSaver(std::size_t tasks_size, const std::string& filename);
    ~ResultSaver() override;
    
    void update(ResultHolder calc_result) override;
    
private:
    std::size_t tasks_size_ = 0;
    std::ofstream file_;
#ifdef MULTITHREAD
    //ThreadPool thread_pool_{1};
    std::mutex cv_m_;
    std::condition_variable condition_;
    std::thread thread_;
    std::vector<ResultHolder> results;
#endif
};

class StreamLogger : public ISubscriber {
public:
    explicit StreamLogger(std::ostream& out = std::cout)
        : out_(out)
    {}
    
    void update(ResultHolder calc_result) override;
    
private:
    std::ostream& out_;
};

class StreamSaver : public ISubscriber {
public:
    explicit StreamSaver(std::ostream& out = std::cout)
        : out_(out)
    {}
    
    void update(ResultHolder calc_result) override;
    
private:
    std::ostream& out_;
};

class PercentLogger : public ISubscriber {
public:
    explicit PercentLogger(std::size_t tasks_size, std::ostream& out = std::cout)
            : tasks_size_(tasks_size), out_(out)
    {}

    void update(ResultHolder calc_result) override;

private:
    std::size_t tasks_size_ = 0;
    std::ostream& out_;
    int cur_percent_ = 0;
};
