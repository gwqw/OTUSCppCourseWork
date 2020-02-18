#pragma once

#include <vector>
#include <utility>
#include <stdexcept>

#include "task_generator_interface.h"
#include "result_saver.h"
#include "project_config.h"

#ifdef THREADPOOL
    #ifdef BOOST
        #include <boost/asio.hpp>
        namespace ba = boost::asio;
    #else
        #include "thread_pool.h"
    #endif
#endif

/**
 * @brief Manages matrix calculation
 */
class CalcTaskMgr {
public:
    explicit CalcTaskMgr(TaskCalculatorHolder task_generator,
                         std::size_t threads_count = 1)
        : threads_count_(threads_count), task_generator_(std::move(task_generator))
#ifdef THREADPOOL
        , thread_pool_(threads_count)
#endif
    {
        if (threads_count_ == 0) {
            throw std::invalid_argument("Threads count can't be zero");
        }
    }
    
    void run();

    void subscribe(SubscriberHolder subscriber);
private:
    std::size_t threads_count_ = 1;
    std::vector<SubscriberHolder> subscribers_;
    TaskCalculatorHolder task_generator_;
#ifdef THREADPOOL
 #ifdef BOOST
    ba::thread_pool thread_pool_;
 #else
    ThreadPool thread_pool_;
 #endif
#endif
    
    // methods
    void notify(CalcResult calc_result) const;
};

template <typename Subscriber, typename ... Args>
void createAndSubscribe(CalcTaskMgr& resp_calculator, Args&&...args) {
    auto s = std::make_unique<Subscriber>(std::forward<Args>(args)...);
    resp_calculator.subscribe(std::move(s));
}
