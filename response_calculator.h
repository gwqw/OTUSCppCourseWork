#pragma once

#include <vector>
#include <utility>

#include "task_generator.h"
#include "result_saver.h"

class ResponseCalculator {
public:
    explicit ResponseCalculator(TaskGeneratorHolder task_generator)
        : task_generator_(std::move(task_generator))
    {}
    
    void run();
    
    bool calulateNextLine();
    
    void subscribe(SubscriberHolder subscriber);
private:
    std::vector<SubscriberHolder> subscribers_;
    TaskGeneratorHolder task_generator_;
    
    // methods
    void notify(const CalcResult& calc_result);
};

template <typename Subscriber, typename ... Args>
void createAndSubscribe(ResponseCalculator& resp_calculator, Args&&...args) {
    auto s = std::make_unique<Subscriber>(std::forward<Args>(args)...);
    resp_calculator.subscribe(std::move(s));
}
