#include "task_generator.h"

#include <stdexcept>

using namespace std;

std::optional<std::size_t> RandomTaskGenerator::getNextTaskNum() {
    if (cur_counter_ < task_input_.task_size) {
        return cur_counter_++;
    } else {
        return nullopt;
    }    
}

CalcResult RandomTaskGenerator::taskCalculation(std::size_t task_num) {
    if (task_num+1 > task_input_.task_size) {
        throw invalid_argument("Task number is greater than number of tasks");
    }
    CalcResult res{task_num};
    res.line.resize(task_input_.task_size);
    for (size_t i = 0; i < task_num+1; ++i) {
        res.line[i] = uniform_distribution_(random_engine_);
    }
    return res;
}


