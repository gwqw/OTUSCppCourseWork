#pragma once

#include <vector>
#include <utility>

#include "task_generator.h"
#include "result_saver.h"
#include "project_config.h"
#ifdef MULTITHREAD
    #ifdef BOOST
        #include <boost/asio.hpp>
        namespace ba = boost::asio;
    #else
        #include "thread_pool.h"
        namespace ba;
    #endif
#endif

class ResponseCalculator {
public:
    explicit ResponseCalculator(TaskGeneratorHolder task_generator)
        : task_generator_(std::move(task_generator))
    {}
    
    void run();

    void subscribe(SubscriberHolder subscriber);
private:
    static constexpr int THREADS_NUM = 4;
    std::vector<SubscriberHolder> subscribers_;
    TaskGeneratorHolder task_generator_;
#ifdef MULTITHREAD
 #ifdef BOOST
    ba::thread_pool thread_pool_{THREADS_NUM};
 #else
    ThreadPool thread_pool_{THREADS_NUM};
 #endif
#endif
    
    // methods
    void notify(CalcResult calc_result) const;
};

template <typename Subscriber, typename ... Args>
void createAndSubscribe(ResponseCalculator& resp_calculator, Args&&...args) {
    auto s = std::make_unique<Subscriber>(std::forward<Args>(args)...);
    resp_calculator.subscribe(std::move(s));
}
