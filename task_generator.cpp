#include "task_generator.h"

#include <stdexcept>
#include <cmath>

#ifndef BOOST
#include "object_pool.h"
#endif

using namespace std;

ResultHolder SimpleTaskCalculator::taskCalculation(std::size_t task_num) {
    if (task_num+1 > task_input_.task_size) {
        throw invalid_argument("Task number is greater than number of tasks");
    }
#ifndef BOOST
    auto& object_pool = ObjectPool<CalcResult>::getSingletone();
    auto res = object_pool.allocate();
    //auto res = object_pool.construct(task_num, task_input_.task_size);
    res->task_num = task_num;
    if (res->line.size() == task_input_.task_size) {
        fill(res->line.begin(), res->line.end(), 0);
    } else {
        res->line.resize(task_input_.task_size);
    }
#else
    auto res = make_shared<CalcResult>(task_num, task_input_.task_size);
#endif

    for (size_t i = 0; i <= task_num; ++i) {
        double value = 1;
        for (int j = 0; j < task_input_.complexity; ++j) {
            value *= exp(double(i + j) / double(task_input_.complexity + task_num));
        }
        res->line[i] = value;
    }
    return res;
}

std::size_t SimpleTaskCalculator::getTasksNumber() const {
    return task_input_.task_size;
}


