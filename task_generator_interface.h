#pragma once

#include <vector>
#include <memory>

#include "project_config.h"

#ifdef BOOST
#include <boost/pool/pool_alloc.hpp>
#endif

/* interface */
struct CalcResult {
    std::size_t task_num = 0;
#ifdef BOOST
    std::vector<double, boost::pool_allocator<double>> line;
#else
    std::vector<double> line;
#endif
    CalcResult() = default;
    CalcResult(std::size_t task_num, std::size_t tasks_size)
        : task_num(task_num), line(tasks_size) {}
};

using ResultHolder = std::shared_ptr<CalcResult>;

class ITaskCalculator {
public:
    virtual ~ITaskCalculator() = default;
    virtual ResultHolder taskCalculation(std::size_t task_num) = 0;
    [[nodiscard]] virtual std::size_t getTasksNumber() const = 0;
};
using TaskCalculatorHolder = std::unique_ptr<ITaskCalculator>;