#pragma once

#include <atomic>
#include <vector>
#include <random>

using CalcResult = std::vector<double>;

struct TaskInput {
    std::size_t task_size = 0;
};

class TaskGenerator {
public:
    TaskGenerator(const TaskInput& task_input)
        : task_input_(task_input)
    {}
    std::size_t getNextTaskNum();
    CalcResult taskCalculation(std::size_t task_num);
private:
    TaskInput task_input_;
    std::atomic_size_t cur_counter_ = 0;
    std::default_random_engine random_engine_;
    std::uniform_real_distribution<double> uniform_distribution_{0, 1};
};


