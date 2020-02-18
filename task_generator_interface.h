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