#include "task_generator.h"

using namespace std;

std::size_t TaskGenerator::getNextTaskNum() {
    return ++cur_counter_;
}

CalcResult TaskGenerator::taskCalculation(std::size_t task_num) {
    CalcResult res;
    res.resize(task_input_.task_size);
    for (auto& i : res) {
        i = uniform_distribution_(random_engine_);
    }
    return res;
}


