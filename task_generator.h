#pragma once

#include "task_generator_interface.h"

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
    [[nodiscard]] std::size_t getTasksNumber() const override {
        return task_input_.task_size;
    }
private:
    TaskInput task_input_;
};

class TestTaskCalculator : public ITaskCalculator {
public:
    explicit TestTaskCalculator(const TaskInput& task_input)
            : task_input_(task_input)
    {}

    CalcResult taskCalculation(std::size_t task_num) override;

    [[nodiscard]] std::size_t getTasksNumber() const override {
        return task_input_.task_size;
    }
private:
    TaskInput task_input_;
};
