#include "task_generator.h"

#include <stdexcept>
#include <cmath>

using namespace std;

CalcResult SimpleTaskCalculator::taskCalculation(std::size_t task_num) {
    if (task_num+1 > task_input_.task_size) {
        throw invalid_argument("Task number is greater than number of tasks");
    }
    CalcResult res{task_num};
    res.line.resize(task_input_.task_size);
    for (size_t i = 0; i <= task_num; ++i) {
        double value = 1;
        for (int j = 0; j < task_input_.complexity; ++j) {
            value *= exp(double(i + j) / double(task_input_.complexity + task_num));
        }
        res.line[i] = value;
    }
    return res;
}

std::size_t SimpleTaskCalculator::getTasksNumber() const {
    return task_input_.task_size;
}


