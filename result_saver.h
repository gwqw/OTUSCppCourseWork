#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include "task_generator.h"

class ISubscriber {
public:
    virtual ~ISubscriber() = default;
    virtual void update(ResultHolder calc_result) = 0;
};

using SubscriberHolder = std::unique_ptr<ISubscriber>;

class ResultSaver : public ISubscriber {
public:
    explicit ResultSaver(const std::string& filename) 
        : file_(filename, std::ios_base::binary) 
    {}
    
    void update(ResultHolder calc_result) override;
    
private:
    std::vector<ResultHolder> results_;
    std::ofstream file_;
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
