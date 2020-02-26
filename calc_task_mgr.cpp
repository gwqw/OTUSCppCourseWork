#include "calc_task_mgr.h"

#include <future>

using namespace std;

void CalcTaskMgr::run()
{
#ifdef MULTITHREAD
    vector<future<void>> futures;
    futures.reserve(threads_count_);
    auto calc_func = [this](size_t first_task_num, size_t tasks_number, size_t threads_count){
        for (size_t task_num = first_task_num; task_num < tasks_number; task_num += threads_count) {
            auto calc_result = task_generator_->taskCalculation(task_num);
            notify(move(calc_result));
        }
    };
    for (size_t first_task_num = 0; first_task_num < threads_count_; ++first_task_num) {
        futures.emplace_back(async(
                launch::async, calc_func, first_task_num,
                task_generator_->getTasksNumber(), threads_count_)
        );
    }
#else // SINGLE_THREAD
    auto tasks_number = task_generator_->getTasksNumber();
    for (size_t task_num = 0; task_num < tasks_number; ++task_num) {
        auto calc_result = task_generator_->taskCalculation(task_num);
        notify(move(calc_result));
    }
#endif // MULTITHREAD
}

void CalcTaskMgr::subscribe(SubscriberHolder subscriber)
{
    subscribers_.push_back(move(subscriber));
}

void CalcTaskMgr::notify(ResultHolder calc_result) const {
    for (auto& s : subscribers_) {
        s->update(calc_result);
    }
}