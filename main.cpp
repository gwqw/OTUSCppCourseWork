#include <iostream>

#include <memory>

#include "task_generator.h"
#include "response_calculator.h"
#include "result_saver.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        auto task_generator = make_unique<RandomTaskGenerator>(TaskInput{3});
        ResponseCalculator resp_calculator(move(task_generator));
        createAndSubscribe<StreamSaver>(resp_calculator, cout);
        createAndSubscribe<ResultSaver>(resp_calculator, "test.mtx");
        resp_calculator.run();
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}
