#include "response_calculator.h"

#include <future>

using namespace std;

void ResponseCalculator::run()
{
#ifdef MULTITHREAD

#ifdef THREADPOOL
#ifndef BOOST
    auto calc_func = [this](size_t first_task_num, size_t tasks_number, size_t threads_count){
        for (size_t task_num = first_task_num; task_num < tasks_number; task_num += threads_count) {
            auto calc_result = task_generator_->taskCalculation(task_num);
            notify(move(calc_result));
        }
    };
#endif

    for (size_t first_task_num = 0; first_task_num < THREADS_NUM; ++first_task_num) {
#ifdef BOOST
        auto tasks_number = task_generator_->getTasksNumber();
        auto threads_count = THREADS_NUM;
        auto calc_func = [this, first_task_num, tasks_number, threads_count](){
            for (size_t task_num = first_task_num; task_num < tasks_number; task_num += threads_count) {
                auto calc_result = task_generator_->taskCalculation(task_num);
                notify(move(calc_result));
            }
        };
        ba::post(thread_pool_, calc_func);
#else
        post(thread_pool_, calc_func, first_task_num, task_generator_->getTasksNumber(),
                THREADS_NUM);
#endif
    }
#ifdef BOOST
    thread_pool_.join();
#endif // BOOST
#else // FUTURES
    vector<future<void>> futures;
    futures.reserve(THREADS_NUM);
    auto calc_func = [this](size_t first_task_num, size_t tasks_number, size_t threads_count){
        for (size_t task_num = first_task_num; task_num < tasks_number; task_num += threads_count) {
            auto calc_result = task_generator_->taskCalculation(task_num);
            notify(move(calc_result));
        }
    };
    for (size_t first_task_num = 0; first_task_num < THREADS_NUM; ++first_task_num) {
        futures.emplace_back(async(
                launch::async, calc_func, first_task_num,
                task_generator_->getTasksNumber(), THREADS_NUM)
        );
    }
#endif // THREADPOOL

#else // SINGLE_THREAD
    auto tasks_number = task_generator_->getTasksNumber();
    for (size_t task_num = 0; task_num < tasks_number; ++task_num) {
        auto calc_result = task_generator_->taskCalculation(task_num);
        notify(move(calc_result));
    }
#endif // MULTITHREAD
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