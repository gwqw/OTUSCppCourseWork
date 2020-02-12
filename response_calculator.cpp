#include "response_calculator.h"

using namespace std;

void ResponseCalculator::run()
{
#ifdef MULTITHREAD
    auto tasks_number = task_generator_->getTasksNumber();
#ifndef BOOST
    auto calc_func = [this](size_t task_num){
        auto calc_result = task_generator_->taskCalculation(task_num);
        notify(move(calc_result));
    };
#endif

    for (size_t task_num = 0; task_num < tasks_number; ++task_num) {
#ifdef BOOST
        auto calc_func = [this, task_num](){
            auto calc_result = task_generator_->taskCalculation(task_num);
            notify(move(calc_result));
        };
        ba::post(thread_pool_, calc_func);
#else
        post(thread_pool_, calc_func, task_num);
#endif
    }
#else
    auto tasks_number = task_generator_->getTasksNumber();
    for (size_t task_num = 0; task_num < tasks_number; ++task_num) {
        auto calc_result = task_generator_->taskCalculation(task_num);
        notify(move(calc_result));
    }
#endif
}

void ResponseCalculator::subscribe(SubscriberHolder subscriber)
{
    subscribers_.push_back(move(subscriber));
}

void ResponseCalculator::notify(CalcResult calc_result) const {
    auto result = make_shared<CalcResult>(move(calc_result));
    for (auto& s : subscribers_) {
        s->update(result);
    }
}