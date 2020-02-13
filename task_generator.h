#pragma once

#include <vector>
#include <memory>

struct CalcResult {
    std::size_t task_num = 0;
    std::vector<double> line;
};

using ResultHolder = std::shared_ptr<CalcResult>;

class ITaskGenerator {
public:
    virtual ~ITaskGenerator() = default;
    virtual CalcResult taskCalculation(std::size_t task_num) = 0;
    [[nodiscard]] virtual std::size_t getTasksNumber() const = 0;
};
using TaskGeneratorHolder = std::unique_ptr<ITaskGenerator>;


// implementation
struct TaskInput {
    std::size_t task_size = 0;
    int complexity = 1;
};

class SimpleTaskGenerator : public ITaskGenerator {
public:
    explicit SimpleTaskGenerator(const TaskInput& task_input)
        : task_input_(task_input)
    {}

    CalcResult taskCalculation(std::size_t task_num) override;
    [[nodiscard]] std::size_t getTasksNumber() const override;
private:
    TaskInput task_input_;
};
