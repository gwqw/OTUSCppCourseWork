#include "response_calculator.h"

using namespace std;

bool ResponseCalculator::calulateNextLine()
{
    auto task_num_opt = task_generator_->getNextTaskNum();
    if (!task_num_opt) return false;
    auto task_num = *task_num_opt;
    auto calc_result = task_generator_->taskCalculation(task_num);
    notify(move(calc_result));
    return true;
}

void ResponseCalculator::run() 
{
    while (calulateNextLine());
}

void ResponseCalculator::subscribe(SubscriberHolder subscriber)
{
    subscribers_.push_back(move(subscriber));
}

void ResponseCalculator::notify(CalcResult calc_result) {
    auto result = make_shared<CalcResult>(move(calc_result));
    for (auto& s : subscribers_) {
        s->update(result);
    }
}