#pragma once

#include <vector>
#include <memory>

/* interface */
struct CalcResult {
    std::size_t task_num = 0;
    std::vector<double> line;
};

using ResultHolder = std::shared_ptr<CalcResult>;

class ITaskCalculator {
public:
    virtual ~ITaskCalculator() = default;
    virtual CalcResult taskCalculation(std::size_t task_num) = 0;
    [[nodiscard]] virtual std::size_t getTasksNumber() const = 0;
};
using TaskCalculatorHolder = std::unique_ptr<ITaskCalculator>;


/* implementation */
struct TaskInput {
    std::size_t task_size = 0;
    int complexity = 1;
};

class SimpleTaskCalculator : public ITaskCalculator {
public:
    explicit SimpleTaskCalculator(const TaskInput& task_input)
        : task_input_(task_input)
    {}

    CalcResult taskCalculation(std::size_t task_num) override;
    [[nodiscard]] std::size_t getTasksNumber() const override;
private:
    TaskInput task_input_;
};
