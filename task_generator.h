#pragma once

#include <atomic>
#include <vector>
#include <random>
#include <memory>
#include <optional>

struct CalcResult {
    std::size_t task_num = 0;
    std::vector<double> line;
};

using ResultHolder = std::shared_ptr<CalcResult>;

class ITaskGenerator {
public:
    virtual ~ITaskGenerator() = default;
    virtual std::optional<std::size_t> getNextTaskNum() = 0;
    virtual CalcResult taskCalculation(std::size_t task_num) = 0;
};
using TaskGeneratorHolder = std::unique_ptr<ITaskGenerator>;

struct TaskInput {
    std::size_t task_size = 0;
};

class RandomTaskGenerator : public ITaskGenerator {
public:
    RandomTaskGenerator(const TaskInput& task_input)
        : task_input_(task_input)
    {}
    std::optional<std::size_t> getNextTaskNum() override;
    CalcResult taskCalculation(std::size_t task_num) override;
private:
    TaskInput task_input_;
    std::atomic_size_t cur_counter_ = 0;
    std::default_random_engine random_engine_;
    std::uniform_real_distribution<double> uniform_distribution_{0, 1};
};
